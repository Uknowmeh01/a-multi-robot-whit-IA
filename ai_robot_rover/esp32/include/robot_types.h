#pragma once

#include <Arduino.h>
#include <math.h>

enum class RobotMode : uint8_t { MANUAL, BLUETOOTH, AUTONOMOUS, AI };
enum class MotionAction : uint8_t {
  NONE,
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  ROTATE_LEFT,
  ROTATE_RIGHT
};
enum class CommandSource : uint8_t { SERIAL_LINK, BLUETOOTH_LINK };
enum class CommandType : uint8_t {
  INVALID,
  MOVE,
  STOP,
  EMERGENCY_STOP,
  CLEAR_EMERGENCY,
  SET_MODE,
  SET_SPEED,
  SET_LED,
  STATUS,
  HELP
};

struct ParsedCommand {
  CommandType type = CommandType::INVALID;
  CommandSource source = CommandSource::SERIAL_LINK;
  MotionAction motion = MotionAction::NONE;
  RobotMode mode = RobotMode::MANUAL;
  uint8_t speed = 0;
  bool enabled = false;
  String error;
};

struct UltrasonicReadings {
  float left = NAN;
  float center = NAN;
  float right = NAN;
  bool leftHealthy = false;
  bool centerHealthy = false;
  bool rightHealthy = false;
};

struct SensorReadings {
  float lightLux = NAN;
  uint16_t soundRaw = 0;
  float soundAverage = 0.0F;
  bool lightHealthy = false;
  bool clapDetected = false;
};

inline const char* modeName(RobotMode mode) {
  switch (mode) {
    case RobotMode::MANUAL: return "MANUAL";
    case RobotMode::BLUETOOTH: return "BLUETOOTH";
    case RobotMode::AUTONOMOUS: return "AUTONOMOUS";
    case RobotMode::AI: return "AI";
  }
  return "UNKNOWN";
}
