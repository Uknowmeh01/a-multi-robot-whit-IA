"""Serveur Flask-SocketIO local et API de controle du robot."""

from __future__ import annotations

from typing import Any

from flask import Flask, jsonify, render_template, request
from flask_socketio import SocketIO, emit

import config
from logger import log
from robot_controller import RobotController


class WebServer:
    def __init__(self, robot: RobotController) -> None:
        self.robot = robot
        self.app = Flask(
            __name__,
            template_folder=str(config.BASE_DIR / "templates"),
            static_folder=str(config.BASE_DIR / "static"),
        )
        self.app.config.update(SECRET_KEY=config.WEB_SECRET_KEY, JSON_SORT_KEYS=False)
        self.socketio = SocketIO(
            self.app,
            async_mode="threading",
            cors_allowed_origins=None,
            ping_interval=5,
            ping_timeout=8,
            logger=False,
            engineio_logger=False,
        )
        self._register_routes()
        self._register_socket_events()

    def _register_routes(self) -> None:
        @self.app.get("/")
        def index():
            return render_template("index.html", max_speed=config.MAX_SPEED)

        @self.app.get("/api/status")
        def api_status():
            return jsonify(self.robot.snapshot())

        @self.app.get("/health")
        def health():
            state = self.robot.snapshot()
            return jsonify({"ok": True, "serial_connected": state["serial_connected"]})

    def _safe_event(self, callback, *args: Any) -> None:
        try:
            callback(*args)
            emit("command_ack", {"ok": True})
        except (ValueError, TypeError, RuntimeError) as exc:
            log.warning("Commande Web refusee (%s): %s", request.sid, exc)
            emit("command_error", {"message": str(exc)})
        except Exception:
            log.exception("Erreur commande Web")
            emit("command_error", {"message": "Erreur interne; robot arrete"})
            self.robot.stop(urgent=True)

    def _register_socket_events(self) -> None:
        @self.socketio.on("connect")
        def connected():
            self.robot.phone_connected(request.sid)
            emit("telemetry", self.robot.snapshot())

        @self.socketio.on("disconnect")
        def disconnected():
            self.robot.phone_disconnected(request.sid)

        @self.socketio.on("heartbeat")
        def heartbeat(_payload=None):
            self.robot.phone_heartbeat(request.sid)

        @self.socketio.on("move")
        def move(payload):
            data = payload if isinstance(payload, dict) else {}
            self.robot.phone_heartbeat(request.sid)
            self._safe_event(
                self.robot.motion,
                str(data.get("action", "")),
                int(data["speed"]) if "speed" in data else None,
                request.sid,
            )

        @self.socketio.on("stop")
        def stop(_payload=None):
            self.robot.phone_heartbeat(request.sid)
            self.robot.stop(urgent=True)

        @self.socketio.on("emergency_stop")
        def emergency_stop(_payload=None):
            self.robot.emergency_stop(f"Web:{request.sid}")

        @self.socketio.on("clear_emergency")
        def clear_emergency(_payload=None):
            self._safe_event(self.robot.clear_emergency)

        @self.socketio.on("set_speed")
        def set_speed(payload):
            data = payload if isinstance(payload, dict) else {}
            self._safe_event(self.robot.set_speed, int(data.get("speed", 0)))

        @self.socketio.on("set_mode")
        def set_mode(payload):
            data = payload if isinstance(payload, dict) else {}
            self._safe_event(self.robot.set_mode, str(data.get("mode", "")), f"Web:{request.sid}")

        @self.socketio.on("set_led")
        def set_led(payload):
            data = payload if isinstance(payload, dict) else {}
            self._safe_event(self.robot.set_leds, bool(data.get("enabled", False)))

    def _broadcast_loop(self) -> None:
        period = 1.0 / max(config.TELEMETRY_BROADCAST_HZ, 1.0)
        while True:
            self.socketio.emit("telemetry", self.robot.snapshot())
            self.socketio.sleep(period)

    def run(self) -> None:
        self.socketio.start_background_task(self._broadcast_loop)
        log.info("Interface Web: http://0.0.0.0:%d", config.WEB_PORT)
        self.socketio.run(
            self.app,
            host=config.WEB_HOST,
            port=config.WEB_PORT,
            debug=config.WEB_DEBUG,
            use_reloader=False,
            allow_unsafe_werkzeug=True,
        )
