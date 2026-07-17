#pragma once

#include <Arduino.h>
#include "config.h"
#include "robot_types.h"

class UltrasonicManager {
 public:
  void begin();
  bool update();
  const UltrasonicReadings& readings() const;
  bool emergencyObstacle() const;
  bool closeObstacle() const;
  bool sensorFault() const;

 private:
  struct Sensor {
    uint8_t trig;
    uint8_t echo;
    float samples[Config::ULTRASONIC_MEDIAN_SAMPLES] = {NAN, NAN, NAN, NAN, NAN};
    uint8_t sampleIndex = 0;
    uint8_t validCount = 0;
    uint8_t failures = 0;
    float filtered = NAN;
  };

  Sensor sensors_[3] = {
      {Config::US_LEFT_TRIG_PIN, Config::US_LEFT_ECHO_PIN},
      {Config::US_CENTER_TRIG_PIN, Config::US_CENTER_ECHO_PIN},
      {Config::US_RIGHT_TRIG_PIN, Config::US_RIGHT_ECHO_PIN},
  };
  UltrasonicReadings readings_;
  uint8_t nextSensor_ = 0;
  uint32_t lastReadMs_ = 0;

  float measureCm(const Sensor& sensor);
  static float median(const Sensor& sensor);
  void refreshReadings();
};
