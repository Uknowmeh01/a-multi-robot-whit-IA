#pragma once

#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>
#include "config.h"
#include "robot_types.h"

class SensorManager {
 public:
  void begin();
  bool update();
  const SensorReadings& readings() const;
  bool consumeClapEvent();

 private:
  BH1750 lightMeter_;
  SensorReadings readings_;
  uint32_t lastLightMs_ = 0;
  uint32_t lastSoundMs_ = 0;
  uint32_t lastClapMs_ = 0;
  bool clapPending_ = false;
};
