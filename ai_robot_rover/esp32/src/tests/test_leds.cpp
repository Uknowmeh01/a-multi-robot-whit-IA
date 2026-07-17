#include <Arduino.h>
#include "config.h"
#include "led_controller.h"

LedController leds;
uint8_t stage = 0;
uint32_t changedAt = 0;

void applyStage() {
  leds.setEmergency(false); leds.setError(false); leds.setObstacle(false); leds.setConnected(false);
  switch (stage) {
    case 0: Serial.println("Vert normal"); leds.setMode(RobotMode::MANUAL); break;
    case 1: Serial.println("Bleu connexion"); leds.setConnected(true); break;
    case 2: Serial.println("Jaune autonome"); leds.setMode(RobotMode::AUTONOMOUS); break;
    case 3: Serial.println("Bleu clignotant IA"); leds.setMode(RobotMode::AI); break;
    case 4: Serial.println("Orange obstacle"); leds.setObstacle(true); break;
    case 5: Serial.println("Rouge erreur"); leds.setError(true); break;
    default: Serial.println("Rouge clignotant urgence"); leds.setEmergency(true); stage = 6; break;
  }
}

void setup() { Serial.begin(Config::SERIAL_BAUD); leds.begin(); applyStage(); }
void loop() {
  leds.update();
  if (millis() - changedAt >= 1800 && stage < 6) { changedAt = millis(); ++stage; applyStage(); }
}
