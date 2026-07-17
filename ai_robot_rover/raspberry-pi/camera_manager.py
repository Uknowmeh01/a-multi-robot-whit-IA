"""Gestion Picamera2 tolerante a l'absence ou a la deconnexion de la camera."""

from __future__ import annotations

import threading
import time
from datetime import datetime

import cv2
import numpy as np

import config
from logger import log

try:
    from picamera2 import Picamera2, Preview
except ImportError:  # Permet les tests sur une machine non Raspberry Pi.
    Picamera2 = None  # type: ignore[assignment]
    Preview = None  # type: ignore[assignment]


class CameraManager:
    def __init__(self) -> None:
        self._camera = None
        self._lock = threading.RLock()
        self._last_frame_at = 0.0
        self._last_attempt_at = 0.0
        self.available = False
        self.last_error = ""

    def initialize(self) -> bool:
        with self._lock:
            if self.available:
                return True
            self._last_attempt_at = time.monotonic()
            if Picamera2 is None:
                self.last_error = "Picamera2 indisponible"
                log.error(self.last_error)
                return False
            try:
                camera = Picamera2()
                camera_config = camera.create_video_configuration(
                    main={
                        "size": (config.CAMERA_WIDTH, config.CAMERA_HEIGHT),
                        "format": config.CAMERA_FORMAT,
                    },
                    controls={"FrameRate": config.CAMERA_FPS},
                    buffer_count=3,
                )
                camera.configure(camera_config)
                if config.CAMERA_PREVIEW and Preview is not None:
                    camera.start_preview(Preview.QTGL)
                camera.start()
                self._camera = camera
                self.available = True
                self.last_error = ""
                log.info("Camera initialisee en %dx%d", config.CAMERA_WIDTH, config.CAMERA_HEIGHT)
                return True
            except Exception as exc:
                self.last_error = str(exc)
                self.available = False
                self._camera = None
                log.exception("Initialisation camera impossible")
                return False

    def capture_frame(self, resize: tuple[int, int] | None = None) -> np.ndarray | None:
        minimum_period = 1.0 / max(config.CAMERA_FPS, 0.1)
        now = time.monotonic()
        if now - self._last_frame_at < minimum_period:
            return None
        if not self.available:
            if now - self._last_attempt_at >= config.CAMERA_RETRY_S:
                self.initialize()
            if not self.available:
                return None
        with self._lock:
            try:
                frame = self._camera.capture_array()
                self._last_frame_at = time.monotonic()
                if resize and (frame.shape[1], frame.shape[0]) != resize:
                    frame = cv2.resize(frame, resize, interpolation=cv2.INTER_AREA)
                if config.CAMERA_SAVE_DEBUG:
                    config.CAMERA_DEBUG_DIR.mkdir(parents=True, exist_ok=True)
                    stamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
                    cv2.imwrite(str(config.CAMERA_DEBUG_DIR / f"frame_{stamp}.jpg"), cv2.cvtColor(frame, cv2.COLOR_RGB2BGR))
                return frame
            except Exception as exc:
                self.last_error = str(exc)
                self.available = False
                camera, self._camera = self._camera, None
                if camera is not None:
                    try:
                        camera.stop()
                        camera.close()
                    except Exception:
                        pass
                log.exception("Capture camera impossible; nouvelle tentative ulterieure")
                return None

    def close(self) -> None:
        with self._lock:
            camera, self._camera = self._camera, None
            self.available = False
            if camera is None:
                return
            try:
                camera.stop()
                camera.close()
            except Exception:
                log.exception("Erreur pendant la fermeture de la camera")
