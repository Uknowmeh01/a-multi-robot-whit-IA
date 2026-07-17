#include "autonomous_controller.h"

AutonomousController::AutonomousController(MotorController& motors) : motors_(motors) {}

void AutonomousController::changeState(AutonomousState state) {
  if (state_ == state) return;
  state_ = state;
  stateSinceMs_ = millis();
}

void AutonomousController::start() {
  motors_.stopAll(true);
  lastProgressMs_ = millis();
  changeState(AutonomousState::STOP);
}

void AutonomousController::stop() {
  motors_.stopAll(true);
  changeState(AutonomousState::STOP);
}

void AutonomousController::emergencyStop() {
  motors_.emergencyStop();
  changeState(AutonomousState::EMERGENCY_STOP);
}

void AutonomousController::chooseTurn(const UltrasonicReadings& distances) {
  const float left = isfinite(distances.left) ? distances.left : 0.0F;
  const float right = isfinite(distances.right) ? distances.right : 0.0F;
  if (left < Config::MIN_DISTANCE_CM && right < Config::MIN_DISTANCE_CM) {
    turnLeftAfterReverse_ = left >= right;
    changeState(AutonomousState::REVERSE);
    motors_.setMotion(MotionAction::BACKWARD, Config::REVERSE_SPEED);
  } else if (left >= right) {
    changeState(AutonomousState::TURN_LEFT);
    motors_.setMotion(MotionAction::ROTATE_LEFT, Config::TURN_SPEED);
  } else {
    changeState(AutonomousState::TURN_RIGHT);
    motors_.setMotion(MotionAction::ROTATE_RIGHT, Config::TURN_SPEED);
  }
}

void AutonomousController::update(const UltrasonicReadings& distances) {
  const uint32_t now = millis();
  if (state_ == AutonomousState::EMERGENCY_STOP) return;
  if (!distances.centerHealthy || !isfinite(distances.center)) {
    motors_.stopAll(true);
    changeState(AutonomousState::BLOCKED);
    return;
  }

  switch (state_) {
    case AutonomousState::STOP:
      if (distances.center > Config::MIN_DISTANCE_CM) {
        changeState(AutonomousState::FORWARD);
        motors_.setMotion(MotionAction::FORWARD, Config::AUTONOMOUS_SPEED);
      } else {
        changeState(AutonomousState::OBSTACLE_DETECTED);
        motors_.stopAll(true);
      }
      break;
    case AutonomousState::FORWARD:
      if (distances.center <= Config::MIN_DISTANCE_CM) {
        motors_.stopAll(true);
        changeState(AutonomousState::OBSTACLE_DETECTED);
      } else if (distances.center <= Config::SLOW_DISTANCE_CM) {
        changeState(AutonomousState::SLOWING);
      } else {
        motors_.setMotion(MotionAction::FORWARD, Config::AUTONOMOUS_SPEED);
        lastProgressMs_ = now;
      }
      break;
    case AutonomousState::SLOWING: {
      if (distances.center <= Config::MIN_DISTANCE_CM) {
        motors_.stopAll(true);
        changeState(AutonomousState::OBSTACLE_DETECTED);
      } else if (distances.center > Config::SLOW_DISTANCE_CM) {
        changeState(AutonomousState::FORWARD);
      } else {
        const float ratio = (distances.center - Config::MIN_DISTANCE_CM) /
                            (Config::SLOW_DISTANCE_CM - Config::MIN_DISTANCE_CM);
        const uint8_t speed = 55 + static_cast<uint8_t>(ratio * (Config::AUTONOMOUS_SPEED - 55));
        motors_.setMotion(MotionAction::FORWARD, speed);
      }
      break;
    }
    case AutonomousState::OBSTACLE_DETECTED:
      if (now - stateSinceMs_ >= 120) chooseTurn(distances);
      break;
    case AutonomousState::REVERSE:
      if (now - stateSinceMs_ >= Config::AUTONOMOUS_REVERSE_MS) {
        motors_.stopAll(true);
        changeState(turnLeftAfterReverse_ ? AutonomousState::TURN_LEFT : AutonomousState::TURN_RIGHT);
        motors_.setMotion(turnLeftAfterReverse_ ? MotionAction::ROTATE_LEFT : MotionAction::ROTATE_RIGHT,
                          Config::TURN_SPEED);
      }
      break;
    case AutonomousState::TURN_LEFT:
    case AutonomousState::TURN_RIGHT:
      if (now - stateSinceMs_ >= Config::AUTONOMOUS_TURN_MS) {
        motors_.stopAll(true);
        changeState(AutonomousState::STOP);
      }
      break;
    case AutonomousState::BLOCKED:
      motors_.stopAll(true);
      if (distances.centerHealthy && distances.center > Config::MIN_DISTANCE_CM) {
        changeState(AutonomousState::STOP);
      }
      break;
    case AutonomousState::EMERGENCY_STOP:
      break;
  }
  if (now - lastProgressMs_ > Config::AUTONOMOUS_STUCK_TIMEOUT_MS &&
      state_ == AutonomousState::FORWARD && distances.center <= Config::SLOW_DISTANCE_CM) {
    motors_.stopAll(true);
    changeState(AutonomousState::BLOCKED);
  }
}

AutonomousState AutonomousController::state() const { return state_; }

const char* AutonomousController::stateName() const {
  switch (state_) {
    case AutonomousState::STOP: return "ARRET";
    case AutonomousState::FORWARD: return "AVANCE";
    case AutonomousState::SLOWING: return "RALENTISSEMENT";
    case AutonomousState::OBSTACLE_DETECTED: return "OBSTACLE_DETECTE";
    case AutonomousState::REVERSE: return "RECUL";
    case AutonomousState::TURN_LEFT: return "TOURNE_GAUCHE";
    case AutonomousState::TURN_RIGHT: return "TOURNE_DROITE";
    case AutonomousState::BLOCKED: return "BLOQUE";
    case AutonomousState::EMERGENCY_STOP: return "ARRET_URGENCE";
  }
  return "INCONNU";
}
