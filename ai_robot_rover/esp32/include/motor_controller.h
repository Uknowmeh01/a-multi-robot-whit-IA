#pragma once

#include <Arduino.h>
#include "config.h"
#include "robot_types.h"

class MotorController {
 public:
  void begin();
  void update();
  void setMotion(MotionAction action, uint8_t speed);
  void setMotorTargets(int16_t frontLeft, int16_t rearLeft,
                       int16_t frontRight, int16_t rearRight);
  void stopAll(bool immediate = false);
  void emergencyStop();
  bool isMoving() const;
  uint8_t commandedSpeed() const;

 private:
  struct Motor {
    uint8_t in1;
    uint8_t in2;
    uint8_t pwmPin;
    uint8_t pwmChannel;
    bool inverted;
    int16_t target = 0;
    int16_t current = 0;
  };

  Motor motors_[4] = {
      {Config::FL_IN1_PIN, Config::FL_IN2_PIN, Config::FL_PWM_PIN, Config::PWM_CHANNEL_FL, Config::FL_INVERTED},
      {Config::RL_IN1_PIN, Config::RL_IN2_PIN, Config::RL_PWM_PIN, Config::PWM_CHANNEL_RL, Config::RL_INVERTED},
      {Config::FR_IN1_PIN, Config::FR_IN2_PIN, Config::FR_PWM_PIN, Config::PWM_CHANNEL_FR, Config::FR_INVERTED},
      {Config::RR_IN1_PIN, Config::RR_IN2_PIN, Config::RR_PWM_PIN, Config::PWM_CHANNEL_RR, Config::RR_INVERTED},
  };
  uint32_t lastRampMs_ = 0;
  uint8_t commandedSpeed_ = 0;

  static int16_t clampSpeed(int16_t value);
  static int16_t rampValue(int16_t current, int16_t target);
  void writeMotor(Motor& motor);
};
