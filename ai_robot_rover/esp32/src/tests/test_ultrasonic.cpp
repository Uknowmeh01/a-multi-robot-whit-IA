#include <Arduino.h>
#include "config.h"
#include "ultrasonic_manager.h"

UltrasonicManager ultrasonics;
uint32_t lastPrint = 0;

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  ultrasonics.begin();
  Serial.println("Test ultrasons. Verifier les diviseurs ECHO 5V -> 3,3V.");
}

void loop() {
  ultrasonics.update();
  if (millis() - lastPrint >= 400) {
    lastPrint = millis();
    const auto& d = ultrasonics.readings();
    Serial.printf("L=%.1f (%d) C=%.1f (%d) R=%.1f (%d) urgence=%d\n",
                  d.left, d.leftHealthy, d.center, d.centerHealthy,
                  d.right, d.rightHealthy, ultrasonics.emergencyObstacle());
  }
}
