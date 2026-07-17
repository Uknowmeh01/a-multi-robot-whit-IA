#!/usr/bin/env python3
"""Test local Flask + Socket.IO sans connexion ESP32 et sans mouvement."""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from robot_controller import RobotController
from web_server import WebServer


def main() -> int:
    robot = RobotController()
    server = WebServer(robot)
    http = server.app.test_client()
    page = http.get("/")
    health = http.get("/health")
    assert page.status_code == 200 and b"AI Robot Rover" in page.data
    assert health.status_code == 200 and health.json["ok"] is True
    ws = server.socketio.test_client(server.app, flask_test_client=http)
    assert ws.is_connected()
    ws.emit("set_speed", {"speed": 80})
    ws.emit("stop")
    received = ws.get_received()
    assert any(event["name"] == "telemetry" for event in received)
    assert robot.snapshot()["speed"] == 80
    ws.disconnect()
    print("OK: page, health, WebSocket, telemetrie, vitesse et STOP")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
