#include "motor_controller.h"

#include <algorithm>

void MotorController::begin() {
  pinMode(Config::MOTOR_STBY_PIN, OUTPUT);
  digitalWrite(Config::MOTOR_STBY_PIN, LOW);
  for (auto& motor : motors_) {
    pinMode(motor.in1, OUTPUT);
    pinMode(motor.in2, OUTPUT);
    digitalWrite(motor.in1, LOW);
    digitalWrite(motor.in2, LOW);
    ledcSetup(motor.pwmChannel, Config::PWM_FREQUENCY, Config::PWM_RESOLUTION_BITS);
    ledcAttachPin(motor.pwmPin, motor.pwmChannel);
    ledcWrite(motor.pwmChannel, 0);
  }
  stopAll(true);
}

int16_t MotorController::clampSpeed(int16_t value) {
  return constrain(value, -static_cast<int16_t>(Config::MAX_SPEED),
                   static_cast<int16_t>(Config::MAX_SPEED));
}

void MotorController::setMotorTargets(int16_t frontLeft, int16_t rearLeft,
                                      int16_t frontRight, int16_t rearRight) {
  motors_[0].target = clampSpeed(frontLeft);
  motors_[1].target = clampSpeed(rearLeft);
  motors_[2].target = clampSpeed(frontRight);
  motors_[3].target = clampSpeed(rearRight);
  commandedSpeed_ = max(max(abs(motors_[0].target), abs(motors_[1].target)),
                        max(abs(motors_[2].target), abs(motors_[3].target)));
  if (commandedSpeed_ > 0) digitalWrite(Config::MOTOR_STBY_PIN, HIGH);
}

void MotorController::setMotion(MotionAction action, uint8_t requestedSpeed) {
  const int16_t speed = min(requestedSpeed, Config::MAX_SPEED);
  const int16_t inner = speed * 45 / 100;
  switch (action) {
    case MotionAction::FORWARD: setMotorTargets(speed, speed, speed, speed); break;
    case MotionAction::BACKWARD: setMotorTargets(-speed, -speed, -speed, -speed); break;
    case MotionAction::TURN_LEFT: setMotorTargets(inner, inner, speed, speed); break;
    case MotionAction::TURN_RIGHT: setMotorTargets(speed, speed, inner, inner); break;
    case MotionAction::ROTATE_LEFT: setMotorTargets(-speed, -speed, speed, speed); break;
    case MotionAction::ROTATE_RIGHT: setMotorTargets(speed, speed, -speed, -speed); break;
    default: stopAll(); break;
  }
}

int16_t MotorController::rampValue(int16_t current, int16_t target) {
  if (current == target) return current;
  if ((current > 0 && target < 0) || (current < 0 && target > 0)) target = 0;
  const uint8_t step = abs(target) > abs(current) ? Config::ACCEL_STEP : Config::DECEL_STEP;
  if (current < target) return std::min<int16_t>(current + step, target);
  return std::max<int16_t>(current - step, target);
}

void MotorController::writeMotor(Motor& motor) {
  int16_t effective = motor.inverted ? -motor.current : motor.current;
  if (effective > 0) {
    digitalWrite(motor.in1, HIGH);
    digitalWrite(motor.in2, LOW);
  } else if (effective < 0) {
    digitalWrite(motor.in1, LOW);
    digitalWrite(motor.in2, HIGH);
  } else {
    digitalWrite(motor.in1, LOW);
    digitalWrite(motor.in2, LOW);
  }
  ledcWrite(motor.pwmChannel, abs(effective));
}

void MotorController::update() {
  const uint32_t now = millis();
  if (now - lastRampMs_ < Config::MOTOR_RAMP_INTERVAL_MS) return;
  lastRampMs_ = now;
  bool active = false;
  for (auto& motor : motors_) {
    motor.current = rampValue(motor.current, motor.target);
    writeMotor(motor);
    active = active || motor.current != 0 || motor.target != 0;
  }
  digitalWrite(Config::MOTOR_STBY_PIN, active ? HIGH : LOW);
}

void MotorController::stopAll(bool immediate) {
  commandedSpeed_ = 0;
  for (auto& motor : motors_) {
    motor.target = 0;
    if (immediate) {
      motor.current = 0;
      writeMotor(motor);
    }
  }
  if (immediate) digitalWrite(Config::MOTOR_STBY_PIN, LOW);
}

void MotorController::emergencyStop() { stopAll(true); }

bool MotorController::isMoving() const {
  for (const auto& motor : motors_) {
    if (motor.current != 0 || motor.target != 0) return true;
  }
  return false;
}

uint8_t MotorController::commandedSpeed() const { return commandedSpeed_; }
