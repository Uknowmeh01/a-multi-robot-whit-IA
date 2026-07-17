"""Configuration centrale du Raspberry Pi. Toutes les valeurs sont surchargeables par variables d'environnement."""

from __future__ import annotations

import os
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent


def _bool(name: str, default: bool) -> bool:
    return os.getenv(name, str(default)).strip().lower() in {"1", "true", "yes", "on"}


# Liaison ESP32
SERIAL_PORT = os.getenv("ROBOT_SERIAL_PORT", "/dev/ttyUSB0")
SERIAL_FALLBACK_PORTS = tuple(
    p.strip()
    for p in os.getenv(
        "ROBOT_SERIAL_FALLBACKS", "/dev/ttyACM0,/dev/serial0,/dev/ttyUSB1"
    ).split(",")
    if p.strip()
)
SERIAL_BAUD = int(os.getenv("ROBOT_SERIAL_BAUD", "115200"))
SERIAL_READ_TIMEOUT_S = float(os.getenv("ROBOT_SERIAL_READ_TIMEOUT", "0.20"))
SERIAL_RECONNECT_DELAY_S = float(os.getenv("ROBOT_SERIAL_RECONNECT_DELAY", "1.0"))
SERIAL_HEARTBEAT_S = float(os.getenv("ROBOT_SERIAL_HEARTBEAT", "0.8"))
SERIAL_LINK_TIMEOUT_S = float(os.getenv("ROBOT_SERIAL_LINK_TIMEOUT", "2.5"))

# Serveur Web local
WEB_HOST = os.getenv("ROBOT_WEB_HOST", "0.0.0.0")
WEB_PORT = int(os.getenv("ROBOT_WEB_PORT", "5000"))
WEB_DEBUG = _bool("ROBOT_WEB_DEBUG", False)
WEB_SECRET_KEY = os.getenv("ROBOT_WEB_SECRET_KEY", "local-robot-control-change-me")
PHONE_COMMAND_TIMEOUT_S = float(os.getenv("ROBOT_PHONE_TIMEOUT", "0.75"))
TELEMETRY_BROADCAST_HZ = float(os.getenv("ROBOT_TELEMETRY_HZ", "5"))

# Mouvement. Le firmware applique aussi sa propre limite, qui reste prioritaire.
DEFAULT_SPEED = int(os.getenv("ROBOT_DEFAULT_SPEED", "120"))
MAX_SPEED = int(os.getenv("ROBOT_MAX_SPEED", "210"))
MIN_TEST_SPEED = int(os.getenv("ROBOT_MIN_TEST_SPEED", "60"))
ALLOWED_MODES = ("MANUAL", "BLUETOOTH", "AUTONOMOUS", "AI")
MOTION_COMMANDS = {
    "FORWARD": "MOVE,FORWARD,{speed}",
    "BACKWARD": "MOVE,BACKWARD,{speed}",
    "LEFT": "TURN,LEFT,{speed}",
    "RIGHT": "TURN,RIGHT,{speed}",
    "ROTATE_LEFT": "ROTATE,LEFT,{speed}",
    "ROTATE_RIGHT": "ROTATE,RIGHT,{speed}",
}

# Les valeurs doivent rester coherentes avec esp32/include/config.h.
SLOW_DISTANCE_CM = float(os.getenv("ROBOT_SLOW_DISTANCE", "55"))
MIN_DISTANCE_CM = float(os.getenv("ROBOT_MIN_DISTANCE", "28"))
EMERGENCY_DISTANCE_CM = float(os.getenv("ROBOT_EMERGENCY_DISTANCE", "12"))

# Camera Module 3 Wide / Picamera2
CAMERA_WIDTH = int(os.getenv("ROBOT_CAMERA_WIDTH", "640"))
CAMERA_HEIGHT = int(os.getenv("ROBOT_CAMERA_HEIGHT", "480"))
CAMERA_FORMAT = os.getenv("ROBOT_CAMERA_FORMAT", "RGB888")
CAMERA_FPS = float(os.getenv("ROBOT_CAMERA_FPS", "8"))
CAMERA_PREVIEW = _bool("ROBOT_CAMERA_PREVIEW", False)
CAMERA_SAVE_DEBUG = _bool("ROBOT_CAMERA_SAVE_DEBUG", False)
CAMERA_DEBUG_DIR = BASE_DIR / "data" / "debug_images"
CAMERA_RETRY_S = float(os.getenv("ROBOT_CAMERA_RETRY", "5"))

# EfficientDet-Lite0 INT8, environ 4 Mo et adapte au Raspberry Pi 4.
MODEL_PATH = Path(os.getenv("ROBOT_MODEL_PATH", str(BASE_DIR / "models" / "efficientdet_lite0.tflite")))
LABELS_PATH = Path(os.getenv("ROBOT_LABELS_PATH", str(BASE_DIR / "models" / "coco_labels.txt")))
MODEL_DOWNLOAD_URL = os.getenv(
    "ROBOT_MODEL_URL",
    "https://storage.googleapis.com/mediapipemodels/object_detector/efficientdet_lite0/int8/1/efficientdet_lite0.tflite",
)
AI_CONFIDENCE_THRESHOLD = float(os.getenv("ROBOT_AI_CONFIDENCE", "0.40"))
AI_MAX_RESULTS = int(os.getenv("ROBOT_AI_MAX_RESULTS", "10"))
AI_THREADS = int(os.getenv("ROBOT_AI_THREADS", "3"))
AI_TARGET_LABEL = os.getenv("ROBOT_AI_TARGET", "person").strip().lower()
AI_CENTER_TOLERANCE = float(os.getenv("ROBOT_AI_CENTER_TOLERANCE", "0.13"))
AI_FORWARD_SPEED = int(os.getenv("ROBOT_AI_FORWARD_SPEED", "90"))
AI_ROTATE_SPEED = int(os.getenv("ROBOT_AI_ROTATE_SPEED", "70"))
AI_SEARCH_SPEED = int(os.getenv("ROBOT_AI_SEARCH_SPEED", "55"))
AI_TARGET_LOST_S = float(os.getenv("ROBOT_AI_TARGET_LOST", "1.2"))
AI_STOP_DISTANCE_CM = float(os.getenv("ROBOT_AI_STOP_DISTANCE", "32"))
AI_COMMAND_REFRESH_S = float(os.getenv("ROBOT_AI_COMMAND_REFRESH", "0.25"))

# Journaux
LOG_DIR = BASE_DIR / "logs"
LOG_FILE = LOG_DIR / "robot.log"
LOG_LEVEL = os.getenv("ROBOT_LOG_LEVEL", "DEBUG" if WEB_DEBUG else "INFO")
LOG_MAX_BYTES = int(os.getenv("ROBOT_LOG_MAX_BYTES", "2000000"))
LOG_BACKUP_COUNT = int(os.getenv("ROBOT_LOG_BACKUPS", "5"))
