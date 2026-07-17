"""Orchestrateur Raspberry Pi : modes, securite, telemetrie, camera et IA."""

from __future__ import annotations

import math
import threading
import time
from collections import deque
from copy import deepcopy
from typing import Any

import config
from ai_behavior import AIBehavior
from ai_detector import AIDetector
from camera_manager import CameraManager
from logger import log
from mode_manager import ModeManager
from serial_controller import SerialController


class RobotController:
    def __init__(self) -> None:
        self.serial = SerialController()
        self.camera = CameraManager()
        self.detector = AIDetector()
        self.ai_behavior = AIBehavior()
        self._lock = threading.RLock()
        self._shutdown = threading.Event()
        self._workers: list[threading.Thread] = []
        self._phone_sessions: dict[str, float] = {}
        self._motion_owner: str | None = None
        self._last_motion_at = 0.0
        self._active_motion = "STOP"
        self._errors: deque[str] = deque(maxlen=12)
        self._state: dict[str, Any] = {
            "serial_connected": False,
            "serial_port": "",
            "phone_connected": False,
            "bluetooth_connected": False,
            "mode": "MANUAL",
            "speed": min(config.DEFAULT_SPEED, config.MAX_SPEED),
            "distances": {"left": None, "center": None, "right": None},
            "light_lux": None,
            "sound_level": None,
            "objects": [],
            "ai_state": "ARRET",
            "camera_available": False,
            "emergency": False,
            "obstacle": None,
            "errors": [],
            "last_event": "Demarrage",
        }
        self.mode_manager = ModeManager(self.serial, self._stop_and_reset, self._on_mode_change)
        self.serial.add_callback(self._handle_serial_line)

    def start(self) -> None:
        self._shutdown.clear()
        self.serial.start()
        if not self.camera.initialize():
            self._add_error(f"Camera indisponible: {self.camera.last_error}")
        if not self.detector.initialize():
            self._add_error(f"IA indisponible: {self.detector.last_error}")
        self._workers = [
            threading.Thread(target=self._watchdog_loop, name="safety-watchdog", daemon=True),
            threading.Thread(target=self._ai_loop, name="ai-loop", daemon=True),
        ]
        for worker in self._workers:
            worker.start()
        log.info("Controleur robot demarre; moteurs maintenus a l'arret")

    def shutdown(self) -> None:
        log.info("Arret propre demande")
        self.stop(urgent=True)
        self._shutdown.set()
        for worker in self._workers:
            worker.join(timeout=2.0)
        self.camera.close()
        self.serial.close()

    def snapshot(self) -> dict[str, Any]:
        with self._lock:
            state = deepcopy(self._state)
            state["mode"] = self.mode_manager.current
            state["serial_connected"] = self.serial.connected
            state["serial_port"] = self.serial.port
            state["phone_connected"] = bool(self._phone_sessions)
            state["camera_available"] = self.camera.available
            state["errors"] = list(self._errors)
            return state

    def phone_connected(self, session_id: str) -> None:
        with self._lock:
            self._phone_sessions[session_id] = time.monotonic()
            self._state["last_event"] = "Telephone connecte"
        log.info("Client Web connecte: %s", session_id)

    def phone_heartbeat(self, session_id: str) -> None:
        with self._lock:
            if session_id in self._phone_sessions:
                self._phone_sessions[session_id] = time.monotonic()

    def phone_disconnected(self, session_id: str) -> None:
        should_stop = False
        return_to_manual = False
        with self._lock:
            self._phone_sessions.pop(session_id, None)
            should_stop = self._motion_owner == session_id
            return_to_manual = not self._phone_sessions and self.mode_manager.current in {"AUTONOMOUS", "AI"}
            if should_stop:
                self._motion_owner = None
        if should_stop:
            self.stop(urgent=True)
        if return_to_manual:
            self.mode_manager.change("MANUAL", "phone_disconnect")
        log.info("Client Web deconnecte: %s; stop=%s", session_id, should_stop)

    def set_mode(self, mode: str, source: str = "web") -> None:
        self.mode_manager.change(mode, source)

    def set_speed(self, speed: int) -> int:
        value = max(0, min(int(speed), config.MAX_SPEED))
        with self._lock:
            self._state["speed"] = value
        self.serial.send(f"SPEED,{value}")
        return value

    def set_leds(self, enabled: bool) -> None:
        self.serial.send("LED,ON" if enabled else "LED,OFF")
        with self._lock:
            self._state["last_event"] = f"LED {'ON' if enabled else 'OFF'}"

    def motion(self, action: str, speed: int | None, owner: str) -> None:
        normalized = action.strip().upper()
        if normalized not in config.MOTION_COMMANDS:
            raise ValueError("Commande de mouvement inconnue")
        if self.mode_manager.current != "MANUAL":
            raise RuntimeError("Le controle Web des moteurs exige le mode MANUAL")
        with self._lock:
            if self._state["emergency"]:
                raise RuntimeError("Arret d'urgence verrouille")
            if owner not in self._phone_sessions:
                raise RuntimeError("Session telephone inconnue")
            selected_speed = self._state["speed"] if speed is None else int(speed)
            selected_speed = max(0, min(selected_speed, config.MAX_SPEED))
            self._phone_sessions[owner] = time.monotonic()
            self._motion_owner = owner
            self._last_motion_at = time.monotonic()
            self._active_motion = normalized
            self._state["speed"] = selected_speed
        command = config.MOTION_COMMANDS[normalized].format(speed=selected_speed)
        if not self.serial.send(command):
            self.stop(urgent=True)
            raise RuntimeError("Commande non transmise a l'ESP32")

    def stop(self, *, urgent: bool = True) -> None:
        with self._lock:
            self._active_motion = "STOP"
            self._motion_owner = None
            self._last_motion_at = time.monotonic()
        self.serial.clear_pending()
        self.serial.send("STOP", urgent=urgent)

    def emergency_stop(self, source: str = "web") -> None:
        with self._lock:
            self._state["emergency"] = True
            self._state["last_event"] = f"Arret d'urgence ({source})"
            self._active_motion = "STOP"
            self._motion_owner = None
        self.serial.clear_pending()
        self.serial.send("EMERGENCY_STOP", urgent=True)
        log.critical("ARRET D'URGENCE demande par %s", source)

    def clear_emergency(self) -> None:
        with self._lock:
            center = self._state["distances"]["center"]
            if center is not None and center <= config.EMERGENCY_DISTANCE_CM:
                raise RuntimeError("Obstacle encore trop proche")
        self.serial.send("CLEAR_EMERGENCY", urgent=True)

    def _stop_and_reset(self) -> None:
        self.stop(urgent=True)
        self.ai_behavior.reset()
        with self._lock:
            self._state["objects"] = []
            self._state["ai_state"] = "ARRET"

    def _on_mode_change(self, mode: str) -> None:
        with self._lock:
            self._state["mode"] = mode
            self._state["last_event"] = f"Mode {mode}"

    def _add_error(self, message: str) -> None:
        with self._lock:
            self._errors.appendleft(message)
            self._state["last_event"] = message
        log.error(message)

    @staticmethod
    def _number(text: str) -> float | None:
        try:
            value = float(text)
            return value if math.isfinite(value) else None
        except ValueError:
            return None

    def _handle_serial_line(self, line: str) -> None:
        parts = [part.strip() for part in line.split(",")]
        event = parts[0].upper() if parts else ""
        with self._lock:
            self._state["serial_connected"] = self.serial.connected
            self._state["serial_port"] = self.serial.port
            if event == "DISTANCE" and len(parts) >= 3:
                side = parts[1].lower()
                if side in self._state["distances"]:
                    self._state["distances"][side] = self._number(parts[2])
                    if side == "center" and self._state["distances"][side] is not None and \
                            self._state["distances"][side] > config.MIN_DISTANCE_CM:
                        self._state["obstacle"] = None
            elif event == "LIGHT" and len(parts) >= 2:
                self._state["light_lux"] = self._number(parts[1])
            elif event == "SOUND" and len(parts) >= 2:
                self._state["sound_level"] = self._number(parts[1])
            elif event == "MODE" and len(parts) >= 2:
                self.mode_manager.synchronize_from_esp32(parts[1])
            elif event == "BLUETOOTH" and len(parts) >= 2:
                self._state["bluetooth_connected"] = parts[1].upper() == "CONNECTED"
            elif event == "OBSTACLE" and len(parts) >= 2:
                self._state["obstacle"] = parts[1]
            elif event == "EMERGENCY_STOP":
                self._state["emergency"] = True
                self._active_motion = "STOP"
            elif event == "EMERGENCY_CLEARED":
                self._state["emergency"] = False
            elif event == "SPEED" and len(parts) >= 2 and parts[1].isdigit():
                self._state["speed"] = int(parts[1])
            elif event == "STATUS":
                for field in parts[1:]:
                    key, separator, value = field.partition("=")
                    if not separator:
                        continue
                    if key == "MODE":
                        self.mode_manager.synchronize_from_esp32(value)
                    elif key == "SPEED" and value.isdigit():
                        self._state["speed"] = int(value)
                    elif key == "EMERGENCY":
                        self._state["emergency"] = value == "1"
                    elif key == "AUTO_STATE":
                        self._state["autonomous_state"] = value
            elif event == "LINK" and len(parts) >= 2:
                connected = parts[1].upper() == "CONNECTED"
                self._state["serial_connected"] = connected
                if connected:
                    self.serial.send("STOP", urgent=True)
                    self.serial.send(f"MODE,{self.mode_manager.current}", urgent=True)
                else:
                    self._active_motion = "STOP"
            elif event in {"READY", "EVENT"}:
                self._state["last_event"] = line
        if event == "ERROR":
            self._add_error(line)
        elif event == "LINK" and len(parts) >= 2 and parts[1].upper() == "DISCONNECTED":
            self.mode_manager.synchronize_from_esp32("MANUAL")
            self._add_error("Connexion serie perdue; ESP32 doit appliquer son watchdog")

    def _watchdog_loop(self) -> None:
        while not self._shutdown.wait(0.08):
            now = time.monotonic()
            stop_reason = ""
            return_to_manual = False
            with self._lock:
                expired = [sid for sid, seen in self._phone_sessions.items() if now - seen > 2.0]
                for sid in expired:
                    self._phone_sessions.pop(sid, None)
                return_to_manual = bool(expired) and not self._phone_sessions and \
                    self.mode_manager.current in {"AUTONOMOUS", "AI"}
                if self._active_motion != "STOP" and self.mode_manager.current == "MANUAL":
                    owner_seen = self._phone_sessions.get(self._motion_owner or "", 0.0)
                    if not owner_seen or now - owner_seen > config.PHONE_COMMAND_TIMEOUT_S:
                        stop_reason = "watchdog telephone"
                    elif now - self._last_motion_at > config.PHONE_COMMAND_TIMEOUT_S:
                        stop_reason = "commande non renouvelee"
                    elif not self.serial.connected:
                        stop_reason = "liaison serie absente"
            if stop_reason:
                log.warning("STOP securite: %s", stop_reason)
                self.stop(urgent=True)
                with self._lock:
                    self._state["last_event"] = f"STOP securite: {stop_reason}"
            if return_to_manual:
                self.mode_manager.change("MANUAL", "phone_heartbeat_timeout")

    def _ai_loop(self) -> None:
        last_command = 0.0
        while not self._shutdown.wait(0.01):
            if self.mode_manager.current != "AI":
                self._shutdown.wait(0.1)
                continue
            frame = self.camera.capture_frame()
            if frame is None:
                with self._lock:
                    self._state["camera_available"] = False
                    self._state["ai_state"] = "ARRET"
                self.serial.send("STOP", urgent=True)
                self._shutdown.wait(0.2)
                continue
            if not self.detector.available:
                with self._lock:
                    self._state["objects"] = []
                    self._state["ai_state"] = "ARRET"
                self.serial.send("STOP", urgent=True)
                self._shutdown.wait(0.5)
                continue
            detections = self.detector.detect(frame)
            if not self.detector.available:
                self._add_error(f"IA arretee: {self.detector.last_error}")
                self.serial.send("STOP", urgent=True)
                self._shutdown.wait(0.5)
                continue
            with self._lock:
                distances = deepcopy(self._state["distances"])
                self._state["objects"] = detections
                self._state["camera_available"] = self.camera.available
            decision = self.ai_behavior.update(detections, distances)
            with self._lock:
                self._state["ai_state"] = decision["state"]
            now = time.monotonic()
            if now - last_command < config.AI_COMMAND_REFRESH_S:
                continue
            last_command = now
            action = decision["action"]
            speed = max(0, min(int(decision.get("speed", 0)), config.MAX_SPEED))
            if action == "EMERGENCY_STOP":
                self.emergency_stop("IA/ultrason")
            elif action == "STOP":
                self.serial.send("STOP", urgent=True)
            elif action in config.MOTION_COMMANDS:
                self.serial.send(config.MOTION_COMMANDS[action].format(speed=speed))
