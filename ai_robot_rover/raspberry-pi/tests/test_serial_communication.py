#!/usr/bin/env python3
"""Test reel du protocole serie. N'envoie aucun mouvement."""

from __future__ import annotations

import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from serial_controller import SerialController


def main() -> int:
    link = SerialController()
    received: list[str] = []
    link.add_callback(lambda line: (received.append(line), print("ESP32:", line)))
    link.start()
    deadline = time.monotonic() + 8
    try:
        while not link.connected and time.monotonic() < deadline:
            time.sleep(0.1)
        if not link.connected:
            print("ECHEC: ESP32 non detecte")
            return 1
        link.send("STOP", urgent=True)
        link.send("STATUS", urgent=True)
        time.sleep(3)
        ok = any(line.startswith(("STATUS,", "READY,")) for line in received)
        print("OK" if ok else "ECHEC: aucune reponse STATUS")
        return 0 if ok else 2
    finally:
        link.close()


if __name__ == "__main__":
    raise SystemExit(main())
