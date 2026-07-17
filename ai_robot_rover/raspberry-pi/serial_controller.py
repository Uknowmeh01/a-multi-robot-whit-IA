"""Liaison serie robuste avec reconnexion, file de sortie et heartbeat."""

from __future__ import annotations

import queue
import threading
import time
from collections.abc import Callable

import serial
from serial import SerialException

import config
from logger import log

LineCallback = Callable[[str], None]


class SerialController:
    def __init__(self) -> None:
        self._serial: serial.Serial | None = None
        self._stop_event = threading.Event()
        self._thread: threading.Thread | None = None
        self._write_queue: queue.Queue[str] = queue.Queue(maxsize=100)
        self._callbacks: list[LineCallback] = []
        self._state_lock = threading.RLock()
        self._write_lock = threading.RLock()
        self._connected = False
        self._port = ""
        self._last_rx_monotonic = 0.0
        self._last_heartbeat = 0.0

    @property
    def connected(self) -> bool:
        with self._state_lock:
            return self._connected

    @property
    def port(self) -> str:
        with self._state_lock:
            return self._port

    @property
    def last_rx_age_s(self) -> float | None:
        with self._state_lock:
            if not self._last_rx_monotonic:
                return None
            return time.monotonic() - self._last_rx_monotonic

    def add_callback(self, callback: LineCallback) -> None:
        self._callbacks.append(callback)

    def start(self) -> None:
        if self._thread and self._thread.is_alive():
            return
        self._stop_event.clear()
        self._thread = threading.Thread(target=self._run, name="serial-link", daemon=True)
        self._thread.start()

    def send(self, line: str, *, urgent: bool = False) -> bool:
        clean = line.strip().replace("\r", "").replace("\n", "")
        if not clean or len(clean) > 96:
            log.warning("Commande serie refusee: vide ou trop longue")
            return False
        if not self.connected:
            log.warning("Commande non envoyee car la liaison serie est absente: %s", clean.split(",", 1)[0])
            return False
        if urgent:
            self._write_immediately(clean)
            return self.connected
        try:
            self._write_queue.put_nowait(clean)
            return True
        except queue.Full:
            log.error("File serie pleine; STOP immediat")
            self._write_immediately("STOP")
            return False

    def clear_pending(self) -> None:
        while True:
            try:
                self._write_queue.get_nowait()
            except queue.Empty:
                break

    def close(self) -> None:
        self._write_immediately("STOP")
        self._stop_event.set()
        if self._thread:
            self._thread.join(timeout=2.0)
        self._disconnect("shutdown")

    def _candidate_ports(self) -> tuple[str, ...]:
        return tuple(dict.fromkeys((config.SERIAL_PORT, *config.SERIAL_FALLBACK_PORTS)))

    def _connect(self) -> bool:
        for port in self._candidate_ports():
            if self._stop_event.is_set():
                return False
            try:
                link = serial.Serial(
                    port=port,
                    baudrate=config.SERIAL_BAUD,
                    timeout=config.SERIAL_READ_TIMEOUT_S,
                    write_timeout=0.4,
                )
                link.reset_input_buffer()
                link.reset_output_buffer()
                with self._state_lock:
                    self._serial = link
                    self._connected = True
                    self._port = port
                    self._last_rx_monotonic = time.monotonic()
                log.info("ESP32 connecte sur %s a %d bauds", port, config.SERIAL_BAUD)
                self._notify("LINK,CONNECTED," + port)
                self._write_immediately("STOP")
                self._write_immediately("STATUS")
                return True
            except (SerialException, OSError):
                continue
        return False

    def _disconnect(self, reason: str) -> None:
        with self._state_lock:
            was_connected = self._connected
            link, self._serial = self._serial, None
            self._connected = False
            self._port = ""
        if link:
            with self._write_lock:
                try:
                    link.close()
                except OSError:
                    pass
        self.clear_pending()
        if was_connected:
            log.error("Liaison ESP32 perdue: %s", reason)
            self._notify("LINK,DISCONNECTED," + reason)

    def _write_immediately(self, line: str) -> None:
        with self._state_lock:
            link = self._serial
        if not link or not link.is_open:
            return
        try:
            with self._write_lock:
                link.write((line + "\n").encode("ascii", errors="strict"))
                link.flush()
        except (SerialException, OSError, UnicodeError) as exc:
            self._disconnect(f"write:{exc}")

    def _notify(self, line: str) -> None:
        for callback in tuple(self._callbacks):
            try:
                callback(line)
            except Exception:
                log.exception("Erreur dans un callback serie pour %r", line)

    def _run(self) -> None:
        while not self._stop_event.is_set():
            if not self.connected:
                if not self._connect():
                    self._stop_event.wait(config.SERIAL_RECONNECT_DELAY_S)
                    continue
            try:
                with self._state_lock:
                    link = self._serial
                if link is None:
                    continue
                raw = link.readline()
                if raw:
                    line = raw.decode("utf-8", errors="replace").strip()
                    with self._state_lock:
                        self._last_rx_monotonic = time.monotonic()
                    if line:
                        self._notify(line)
                for _ in range(8):
                    try:
                        self._write_immediately(self._write_queue.get_nowait())
                    except queue.Empty:
                        break
                now = time.monotonic()
                if now - self._last_heartbeat >= config.SERIAL_HEARTBEAT_S:
                    self._last_heartbeat = now
                    self._write_immediately("STATUS")
                age = self.last_rx_age_s
                if age is not None and age > config.SERIAL_LINK_TIMEOUT_S:
                    self._disconnect(f"silence_{age:.1f}s")
            except (SerialException, OSError) as exc:
                self._disconnect(f"read:{exc}")
            except Exception:
                log.exception("Erreur inattendue dans la liaison serie")
                self._disconnect("unexpected")
