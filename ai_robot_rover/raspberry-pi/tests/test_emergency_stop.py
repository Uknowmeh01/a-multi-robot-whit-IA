#!/usr/bin/env python3
"""Test reel et interactif de l'arret d'urgence serie."""

from __future__ import annotations

import argparse
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from serial_controller import SerialController


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--confirm-wheels-off-ground", action="store_true")
    args = parser.parse_args()
    if not args.confirm_wheels_off_ground:
        print("Refus: placez les roues hors du sol puis ajoutez --confirm-wheels-off-ground")
        return 2
    link = SerialController()
    lines: list[str] = []
    link.add_callback(lambda line: (lines.append(line), print("ESP32:", line)))
    link.start()
    try:
        deadline = time.monotonic() + 8
        while not link.connected and time.monotonic() < deadline:
            time.sleep(0.1)
        if not link.connected:
            print("ECHEC: ESP32 absent")
            return 1
        link.send("STOP", urgent=True)
        link.send("EMERGENCY_STOP", urgent=True)
        time.sleep(1.0)
        link.send("STATUS", urgent=True)
        time.sleep(1.5)
        latched = any("EMERGENCY=1" in line or line.startswith("EMERGENCY_STOP") for line in lines)
        print("OK: urgence verrouillee" if latched else "ECHEC: verrouillage non confirme")
        if latched and input("Zone sure et obstacle retire ? Taper REARMER pour effacer: ").strip().upper() == "REARMER":
            link.send("CLEAR_EMERGENCY", urgent=True)
            time.sleep(1.0)
        return 0 if latched else 3
    finally:
        link.send("STOP", urgent=True)
        link.close()


if __name__ == "__main__":
    raise SystemExit(main())
