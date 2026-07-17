#include <Arduino.h>
#include "config.h"
#include "motor_controller.h"

MotorController motors;
bool active = false;
uint8_t stage = 0;
uint32_t changedAt = 0;

void applyStage() {
  motors.stopAll(true);
  const uint8_t v = Config::MIN_TEST_SPEED;
  switch (stage) {
    case 0: Serial.println("AVANT"); motors.setMotion(MotionAction::FORWARD, v); break;
    case 1: Serial.println("RECUL"); motors.setMotion(MotionAction::BACKWARD, v); break;
    case 2: Serial.println("PIVOT GAUCHE"); motors.setMotion(MotionAction::ROTATE_LEFT, v); break;
    case 3: Serial.println("PIVOT DROITE"); motors.setMotion(MotionAction::ROTATE_RIGHT, v); break;
    default: active = false; motors.emergencyStop(); Serial.println("TERMINE - moteurs arretes"); break;
  }
  changedAt = millis();
}

void setup() {
  Serial.begin(Config::SERIAL_BAUD); motors.begin(); motors.emergencyStop();
  Serial.println("ROUES HORS DU SOL. Taper START pour quatre mouvements de 1 seconde.");
}

void loop() {
  motors.update();
  if (!active && Serial.available()) {
    String line = Serial.readStringUntil('\n'); line.trim(); line.toUpperCase();
    if (line == "START") { active = true; stage = 0; applyStage(); }
  }
  if (active && millis() - changedAt >= 1200) { ++stage; applyStage(); }
}
