#include "sensor_manager.h"

void SensorManager::begin() {
  Wire.begin(Config::I2C_SDA_PIN, Config::I2C_SCL_PIN);
  readings_.lightHealthy = lightMeter_.begin(
      BH1750::CONTINUOUS_HIGH_RES_MODE, Config::BH1750_ADDRESS, &Wire);
  analogReadResolution(12);
  pinMode(Config::SOUND_ADC_PIN, INPUT);
  readings_.soundRaw = analogRead(Config::SOUND_ADC_PIN);
  readings_.soundAverage = readings_.soundRaw;
}

bool SensorManager::update() {
  const uint32_t now = millis();
  bool changed = false;
  if (now - lastSoundMs_ >= Config::SOUND_SAMPLE_INTERVAL_MS) {
    lastSoundMs_ = now;
    readings_.soundRaw = analogRead(Config::SOUND_ADC_PIN);
    readings_.soundAverage += Config::SOUND_FILTER_ALPHA *
                              (readings_.soundRaw - readings_.soundAverage);
    if (readings_.soundRaw >= Config::SOUND_STRONG_THRESHOLD &&
        now - lastClapMs_ >= Config::CLAP_DEBOUNCE_MS) {
      lastClapMs_ = now;
      clapPending_ = true;
      readings_.clapDetected = true;
    } else {
      readings_.clapDetected = false;
    }
    changed = true;
  }
  if (now - lastLightMs_ >= Config::LIGHT_READ_INTERVAL_MS) {
    lastLightMs_ = now;
    if (readings_.lightHealthy && lightMeter_.measurementReady()) {
      const float lux = lightMeter_.readLightLevel();
      if (isfinite(lux) && lux >= 0.0F) {
        readings_.lightLux = lux;
      } else {
        readings_.lightHealthy = false;
      }
    }
    changed = true;
  }
  return changed;
}

const SensorReadings& SensorManager::readings() const { return readings_; }

bool SensorManager::consumeClapEvent() {
  const bool event = clapPending_;
  clapPending_ = false;
  return event;
}
