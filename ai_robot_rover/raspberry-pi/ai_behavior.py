"""Machine a etats du suivi de cible. Les ultrasons sont toujours examines en premier."""

from __future__ import annotations

import math
import time
from enum import Enum
from typing import Any

import config


class AIState(str, Enum):
    SEARCH = "RECHERCHE"
    TARGET_DETECTED = "CIBLE_DETECTEE"
    ALIGNMENT = "ALIGNEMENT"
    FORWARD_TO_TARGET = "AVANCE_VERS_CIBLE"
    TARGET_LOST = "CIBLE_PERDUE"
    OBSTACLE = "OBSTACLE"
    TOO_CLOSE = "TROP_PROCHE"
    STOP = "ARRET"


class AIBehavior:
    def __init__(self) -> None:
        self.state = AIState.STOP
        self.last_target_at = 0.0
        self.last_target_side = "left"

    def reset(self) -> None:
        self.state = AIState.STOP
        self.last_target_at = 0.0

    @staticmethod
    def _finite(value: Any) -> bool:
        return isinstance(value, (int, float)) and math.isfinite(value)

    def _select_target(self, detections: list[dict[str, Any]]) -> dict[str, Any] | None:
        candidates = detections
        if config.AI_TARGET_LABEL:
            candidates = [
                item for item in detections if str(item.get("label", "")).lower() == config.AI_TARGET_LABEL
            ]
        return max(candidates, key=lambda item: float(item.get("confidence", 0.0)), default=None)

    def update(
        self, detections: list[dict[str, Any]], distances: dict[str, float | None]
    ) -> dict[str, Any]:
        now = time.monotonic()
        center = distances.get("center")
        left = distances.get("left")
        right = distances.get("right")

        if not self._finite(center):
            self.state = AIState.STOP
            return {"state": self.state.value, "action": "STOP", "speed": 0, "reason": "distance_invalid"}
        if float(center) <= config.EMERGENCY_DISTANCE_CM:
            self.state = AIState.TOO_CLOSE
            return {"state": self.state.value, "action": "EMERGENCY_STOP", "speed": 0, "reason": "ultrasonic"}
        if float(center) <= config.MIN_DISTANCE_CM:
            self.state = AIState.OBSTACLE
            turn_left = (float(left) if self._finite(left) else 0.0) >= (float(right) if self._finite(right) else 0.0)
            return {
                "state": self.state.value,
                "action": "ROTATE_LEFT" if turn_left else "ROTATE_RIGHT",
                "speed": config.AI_ROTATE_SPEED,
                "reason": "obstacle",
            }
        if float(center) <= config.AI_STOP_DISTANCE_CM:
            self.state = AIState.TOO_CLOSE
            return {"state": self.state.value, "action": "STOP", "speed": 0, "reason": "target_or_obstacle_close"}

        target = self._select_target(detections)
        if target is None:
            elapsed = now - self.last_target_at if self.last_target_at else float("inf")
            self.state = AIState.TARGET_LOST if elapsed <= config.AI_TARGET_LOST_S else AIState.SEARCH
            direction = "ROTATE_LEFT" if self.last_target_side == "left" else "ROTATE_RIGHT"
            return {"state": self.state.value, "action": direction, "speed": config.AI_SEARCH_SPEED, "target": None}

        self.state = AIState.TARGET_DETECTED
        center_x = float(target.get("center_x", 0.5))
        self.last_target_at = now
        self.last_target_side = "left" if center_x < 0.5 else "right"
        error = center_x - 0.5
        if abs(error) > config.AI_CENTER_TOLERANCE:
            self.state = AIState.ALIGNMENT
            return {
                "state": self.state.value,
                "action": "ROTATE_LEFT" if error < 0 else "ROTATE_RIGHT",
                "speed": config.AI_ROTATE_SPEED,
                "target": target,
            }
        self.state = AIState.FORWARD_TO_TARGET
        speed = config.AI_FORWARD_SPEED
        if float(center) < config.SLOW_DISTANCE_CM:
            speed = max(45, int(speed * (float(center) - config.AI_STOP_DISTANCE_CM) /
                                (config.SLOW_DISTANCE_CM - config.AI_STOP_DISTANCE_CM)))
        return {"state": self.state.value, "action": "FORWARD", "speed": speed, "target": target}
