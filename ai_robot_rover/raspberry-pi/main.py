#!/usr/bin/env python3
"""Point d'entree du robot. Un STOP est envoye au demarrage et a la fermeture."""

from __future__ import annotations

import signal
import sys

from logger import log
from robot_controller import RobotController
from web_server import WebServer


def main() -> int:
    robot = RobotController()
    server = WebServer(robot)
    stopping = False

    def request_stop(signum, _frame) -> None:
        nonlocal stopping
        if stopping:
            return
        stopping = True
        log.warning("Signal %s recu: STOP et fermeture", signum)
        robot.shutdown()
        raise KeyboardInterrupt

    signal.signal(signal.SIGTERM, request_stop)
    signal.signal(signal.SIGINT, request_stop)
    try:
        robot.start()
        robot.stop(urgent=True)
        server.run()
    except KeyboardInterrupt:
        pass
    except Exception:
        log.exception("Crash du programme principal")
        robot.emergency_stop("crash Raspberry Pi")
        return 1
    finally:
        if not stopping:
            robot.shutdown()
    return 0


if __name__ == "__main__":
    sys.exit(main())
