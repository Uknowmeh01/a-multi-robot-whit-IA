#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "config.h"
#include "robot_types.h"

class LedController {
 public:
  void begin();
  void update();
  void setMode(RobotMode mode);
  void setConnected(bool connected);
  void setObstacle(bool obstacle);
  void setError(bool error);
  void setEmergency(bool emergency);
  void setDark(bool dark);
  void setUserEnabled(bool enabled);
  bool userEnabled() const;

 private:
  Adafruit_NeoPixel pixels_{Config::LED_COUNT, Config::LED_DATA_PIN, NEO_GRB + NEO_KHZ800};
  RobotMode mode_ = RobotMode::MANUAL;
  bool connected_ = false;
  bool obstacle_ = false;
  bool error_ = false;
  bool emergency_ = false;
  bool dark_ = false;
  bool userEnabled_ = true;
  bool blinkOn_ = true;
  uint32_t lastBlinkMs_ = 0;
  uint32_t lastColor_ = 0xFFFFFFFF;

  uint32_t desiredColor() const;
  void showColor(uint32_t color);
};
