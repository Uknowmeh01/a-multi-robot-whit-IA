#pragma once

#include <Arduino.h>
#include "config.h"
#include "motor_controller.h"
#include "robot_types.h"

enum class AutonomousState : uint8_t {
  STOP,
  FORWARD,
  SLOWING,
  OBSTACLE_DETECTED,
  REVERSE,
  TURN_LEFT,
  TURN_RIGHT,
  BLOCKED,
  EMERGENCY_STOP
};

class AutonomousController {
 public:
  explicit AutonomousController(MotorController& motors);
  void start();
  void stop();
  void emergencyStop();
  void update(const UltrasonicReadings& distances);
  AutonomousState state() const;
  const char* stateName() const;

 private:
  MotorController& motors_;
  AutonomousState state_ = AutonomousState::STOP;
  uint32_t stateSinceMs_ = 0;
  uint32_t lastProgressMs_ = 0;
  bool turnLeftAfterReverse_ = true;

  void changeState(AutonomousState state);
  void chooseTurn(const UltrasonicReadings& distances);
};
