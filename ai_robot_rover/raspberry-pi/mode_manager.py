"""Arbitre exclusif des modes de controle."""

from __future__ import annotations

import threading
from collections.abc import Callable

import config
from logger import log
from serial_controller import SerialController


class ModeManager:
    def __init__(
        self,
        serial_link: SerialController,
        stop_and_reset: Callable[[], None],
        on_change: Callable[[str], None],
    ) -> None:
        self._serial = serial_link
        self._stop_and_reset = stop_and_reset
        self._on_change = on_change
        self._mode = "MANUAL"
        self._lock = threading.RLock()

    @property
    def current(self) -> str:
        with self._lock:
            return self._mode

    def change(self, new_mode: str, source: str = "system") -> bool:
        mode = str(new_mode).strip().upper()
        if mode not in config.ALLOWED_MODES:
            raise ValueError(f"Mode non autorise: {mode}")
        with self._lock:
            if mode == self._mode:
                return False
            old_mode = self._mode
            self._stop_and_reset()
            self._serial.clear_pending()
            self._serial.send("STOP", urgent=True)
            self._serial.send(f"MODE,{mode}", urgent=True)
            self._mode = mode
        log.info("Mode %s -> %s (source=%s)", old_mode, mode, source)
        self._on_change(mode)
        return True

    def synchronize_from_esp32(self, mode: str) -> None:
        normalized = mode.strip().upper()
        if normalized not in config.ALLOWED_MODES:
            return
        with self._lock:
            changed = normalized != self._mode
            self._mode = normalized
        if changed:
            log.info("Mode synchronise depuis ESP32: %s", normalized)
            self._on_change(normalized)
