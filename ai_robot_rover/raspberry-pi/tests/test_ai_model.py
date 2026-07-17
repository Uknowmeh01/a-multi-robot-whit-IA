#!/usr/bin/env python3
"""Charge le modele et execute une inference sur une image noire ou un fichier fourni."""

from __future__ import annotations

import sys
from pathlib import Path

import cv2
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import config
from ai_detector import AIDetector


def main() -> int:
    detector = AIDetector()
    if not detector.initialize():
        print("ECHEC:", detector.last_error)
        return 1
    if len(sys.argv) > 1:
        bgr = cv2.imread(sys.argv[1])
        if bgr is None:
            print("ECHEC: image illisible")
            return 2
        frame = cv2.cvtColor(bgr, cv2.COLOR_BGR2RGB)
    else:
        frame = np.zeros((config.CAMERA_HEIGHT, config.CAMERA_WIDTH, 3), dtype=np.uint8)
    detections = detector.detect(frame)
    print(f"OK: inference terminee, {len(detections)} detection(s)")
    for detection in detections:
        print(detection)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
