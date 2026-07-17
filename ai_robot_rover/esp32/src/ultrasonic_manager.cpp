#include "ultrasonic_manager.h"

#include <algorithm>

void UltrasonicManager::begin() {
  for (auto& sensor : sensors_) {
    pinMode(sensor.trig, OUTPUT);
    digitalWrite(sensor.trig, LOW);
    pinMode(sensor.echo, INPUT);
  }
}

float UltrasonicManager::measureCm(const Sensor& sensor) {
  digitalWrite(sensor.trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trig, LOW);
  const uint32_t duration = pulseIn(sensor.echo, HIGH, Config::ULTRASONIC_TIMEOUT_US);
  if (duration == 0) return NAN;
  const float cm = duration * 0.0343F / 2.0F;
  if (cm < Config::ULTRASONIC_MIN_VALID_CM || cm > Config::ULTRASONIC_MAX_VALID_CM) return NAN;
  return cm;
}

float UltrasonicManager::median(const Sensor& sensor) {
  float values[Config::ULTRASONIC_MEDIAN_SAMPLES];
  uint8_t count = 0;
  for (float value : sensor.samples) {
    if (isfinite(value)) values[count++] = value;
  }
  if (count == 0) return NAN;
  std::sort(values, values + count);
  return count % 2 ? values[count / 2] : (values[count / 2 - 1] + values[count / 2]) / 2.0F;
}

bool UltrasonicManager::update() {
  const uint32_t now = millis();
  if (now - lastReadMs_ < Config::ULTRASONIC_SENSOR_SPACING_MS) return false;
  lastReadMs_ = now;
  Sensor& sensor = sensors_[nextSensor_];
  const float value = measureCm(sensor);
  if (isfinite(value)) {
    sensor.samples[sensor.sampleIndex] = value;
    sensor.sampleIndex = (sensor.sampleIndex + 1) % Config::ULTRASONIC_MEDIAN_SAMPLES;
    sensor.validCount = std::min<uint8_t>(sensor.validCount + 1, Config::ULTRASONIC_MEDIAN_SAMPLES);
    sensor.failures = 0;
    sensor.filtered = median(sensor);
  } else {
    if (sensor.failures < 255) ++sensor.failures;
    if (sensor.failures >= Config::SENSOR_MAX_CONSECUTIVE_FAILURES) sensor.filtered = NAN;
  }
  nextSensor_ = (nextSensor_ + 1) % 3;
  refreshReadings();
  return true;
}

void UltrasonicManager::refreshReadings() {
  readings_.left = sensors_[0].filtered;
  readings_.center = sensors_[1].filtered;
  readings_.right = sensors_[2].filtered;
  readings_.leftHealthy = sensors_[0].failures < Config::SENSOR_MAX_CONSECUTIVE_FAILURES;
  readings_.centerHealthy = sensors_[1].failures < Config::SENSOR_MAX_CONSECUTIVE_FAILURES;
  readings_.rightHealthy = sensors_[2].failures < Config::SENSOR_MAX_CONSECUTIVE_FAILURES;
}

const UltrasonicReadings& UltrasonicManager::readings() const { return readings_; }

bool UltrasonicManager::emergencyObstacle() const {
  return (isfinite(readings_.left) && readings_.left <= Config::EMERGENCY_DISTANCE_CM) ||
         (isfinite(readings_.center) && readings_.center <= Config::EMERGENCY_DISTANCE_CM) ||
         (isfinite(readings_.right) && readings_.right <= Config::EMERGENCY_DISTANCE_CM);
}

bool UltrasonicManager::closeObstacle() const {
  return isfinite(readings_.center) && readings_.center <= Config::MIN_DISTANCE_CM;
}

bool UltrasonicManager::sensorFault() const {
  return !readings_.leftHealthy || !readings_.centerHealthy || !readings_.rightHealthy;
}
