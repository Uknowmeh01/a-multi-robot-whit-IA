#include <Arduino.h>
#include "config.h"
#include "sensor_manager.h"

SensorManager sensors;
uint32_t lastPrint = 0;

void setup() { Serial.begin(Config::SERIAL_BAUD); sensors.begin(); Serial.println("Test BH1750 I2C"); }
void loop() {
  sensors.update();
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    const auto& s = sensors.readings();
    Serial.printf("LIGHT,%.1f,healthy=%d,dark=%d\n", s.lightLux, s.lightHealthy,
                  isfinite(s.lightLux) && s.lightLux < Config::DARK_THRESHOLD_LUX);
  }
}
