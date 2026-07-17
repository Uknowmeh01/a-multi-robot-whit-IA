#include <Arduino.h>
#include "config.h"
#include "motor_controller.h"
#include "ultrasonic_manager.h"

MotorController motors;
UltrasonicManager ultrasonics;
bool running = false;

void setup() {
  Serial.begin(Config::SERIAL_BAUD); motors.begin(); ultrasonics.begin(); motors.emergencyStop();
  Serial.println("ROUES HORS DU SOL. Placer un objet a moins de la distance d'urgence.");
  Serial.println("Taper START: avance lente, puis arret immediat par ultrason. S force aussi l'arret.");
}

void loop() {
  motors.update(); ultrasonics.update();
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n'); line.trim(); line.toUpperCase();
    if (line == "START" && !ultrasonics.emergencyObstacle()) {
      running = true; motors.setMotion(MotionAction::FORWARD, Config::MIN_TEST_SPEED);
    } else if (line == "S" || line == "STOP") {
      running = false; motors.emergencyStop(); Serial.println("ARRET MANUEL OK");
    }
  }
  if (running && ultrasonics.emergencyObstacle()) {
    running = false; motors.emergencyStop(); Serial.println("ARRET URGENCE ULTRASON OK");
  }
}
