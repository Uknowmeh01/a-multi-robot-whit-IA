#include <Arduino.h>
#include "config.h"
#include "sensor_manager.h"

SensorManager sensors;
uint32_t lastPrint = 0;

void setup() { Serial.begin(Config::SERIAL_BAUD); sensors.begin(); Serial.println("Test son analogique et claquement"); }
void loop() {
  sensors.update();
  if (millis() - lastPrint >= 100) {
    lastPrint = millis();
    const auto& s = sensors.readings();
    Serial.printf("raw=%u average=%.1f%s\n", s.soundRaw, s.soundAverage,
                  sensors.consumeClapEvent() ? " CLAP" : "");
  }
}
