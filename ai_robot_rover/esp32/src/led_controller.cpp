#include "led_controller.h"

void LedController::begin() {
  pixels_.begin();
  pixels_.setBrightness(Config::LED_BRIGHTNESS);
  pixels_.clear();
  pixels_.show();
}

void LedController::setMode(RobotMode mode) { mode_ = mode; }
void LedController::setConnected(bool connected) { connected_ = connected; }
void LedController::setObstacle(bool obstacle) { obstacle_ = obstacle; }
void LedController::setError(bool error) { error_ = error; }
void LedController::setEmergency(bool emergency) { emergency_ = emergency; }
void LedController::setDark(bool dark) { dark_ = dark; }
void LedController::setUserEnabled(bool enabled) { userEnabled_ = enabled; }
bool LedController::userEnabled() const { return userEnabled_; }

uint32_t LedController::desiredColor() const {
  if (emergency_) return blinkOn_ ? pixels_.Color(255, 0, 0) : 0;
  if (error_) return pixels_.Color(255, 0, 0);
  if (obstacle_) return pixels_.Color(255, 70, 0);
  if (!userEnabled_ && !dark_) return 0;
  if (mode_ == RobotMode::AI) return blinkOn_ ? pixels_.Color(0, 40, 255) : 0;
  if (mode_ == RobotMode::AUTONOMOUS) return pixels_.Color(255, 180, 0);
  if (connected_ || mode_ == RobotMode::BLUETOOTH) return pixels_.Color(0, 60, 255);
  return pixels_.Color(0, 180, 20);
}

void LedController::showColor(uint32_t color) {
  for (uint16_t i = 0; i < pixels_.numPixels(); ++i) pixels_.setPixelColor(i, color);
  pixels_.show();
  lastColor_ = color;
}

void LedController::update() {
  const uint32_t now = millis();
  if (now - lastBlinkMs_ >= Config::LED_BLINK_INTERVAL_MS) {
    lastBlinkMs_ = now;
    blinkOn_ = !blinkOn_;
  }
  const uint32_t color = desiredColor();
  if (color != lastColor_) showColor(color);
}
