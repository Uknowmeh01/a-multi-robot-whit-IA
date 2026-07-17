#!/usr/bin/env python3
"""Capture dix images et verifie leurs dimensions."""

from __future__ import annotations

import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from camera_manager import CameraManager


def main() -> int:
    camera = CameraManager()
    if not camera.initialize():
        print("ECHEC:", camera.last_error)
        return 1
    frames = 0
    deadline = time.monotonic() + 6
    try:
        while frames < 10 and time.monotonic() < deadline:
            frame = camera.capture_frame()
            if frame is not None:
                frames += 1
                print(f"Image {frames}: {frame.shape}")
            time.sleep(0.02)
    finally:
        camera.close()
    print(f"{'OK' if frames == 10 else 'ECHEC'}: {frames}/10 images")
    return 0 if frames == 10 else 2


if __name__ == "__main__":
    raise SystemExit(main())
