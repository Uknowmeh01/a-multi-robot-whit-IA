#include <Arduino.h>
#include "config.h"
#include "motor_controller.h"

MotorController motors;
bool running = false;
uint8_t stage = 0;
uint32_t stageSince = 0;

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  motors.begin();
  motors.emergencyStop();
  Serial.println("DANGER: retirer les roues du sol et couper l'alimentation moteurs pour cabler.");
  Serial.println("Ce test active chaque moteur a faible vitesse. Taper START puis Entree.");
}

void loop() {
  motors.update();
  if (!running && Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim(); line.toUpperCase();
    if (line == "START") { running = true; stage = 0; stageSince = millis(); Serial.println("Test demarre"); }
  }
  if (!running) return;
  if (millis() - stageSince < 1400) return;
  stageSince = millis();
  motors.stopAll(true);
  ++stage;
  const int16_t speed = Config::MIN_TEST_SPEED;
  switch (stage) {
    case 1: Serial.println("Avant gauche"); motors.setMotorTargets(speed, 0, 0, 0); break;
    case 2: Serial.println("Arriere gauche"); motors.setMotorTargets(0, speed, 0, 0); break;
    case 3: Serial.println("Avant droit"); motors.setMotorTargets(0, 0, speed, 0); break;
    case 4: Serial.println("Arriere droit"); motors.setMotorTargets(0, 0, 0, speed); break;
    default: motors.emergencyStop(); running = false; Serial.println("TERMINE - moteurs arretes"); break;
  }
}
