#include "bluetooth_controller.h"

#include "serial_protocol.h"

bool BluetoothController::begin() {
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  return false;
#else
  buffer_.reserve(Config::MAX_COMMAND_LENGTH);
  return serialBt_.begin(Config::BLUETOOTH_NAME);
#endif
}

ParsedCommand BluetoothController::parseBluetooth(String line) const {
  ParsedCommand command;
  command.source = CommandSource::BLUETOOTH_LINK;
  line.trim();
  line.toUpperCase();
  if (line == "F") { command.type = CommandType::MOVE; command.motion = MotionAction::FORWARD; }
  else if (line == "B") { command.type = CommandType::MOVE; command.motion = MotionAction::BACKWARD; }
  else if (line == "L") { command.type = CommandType::MOVE; command.motion = MotionAction::TURN_LEFT; }
  else if (line == "R") { command.type = CommandType::MOVE; command.motion = MotionAction::TURN_RIGHT; }
  else if (line == "Q") { command.type = CommandType::MOVE; command.motion = MotionAction::ROTATE_LEFT; }
  else if (line == "E") { command.type = CommandType::MOVE; command.motion = MotionAction::ROTATE_RIGHT; }
  else if (line == "S") command.type = CommandType::STOP;
  else if (line == "STOP") command.type = CommandType::EMERGENCY_STOP;
  else if (line == "RESET" || line == "CLEAR_EMERGENCY") command.type = CommandType::CLEAR_EMERGENCY;
  else if (line == "M0") { command.type = CommandType::SET_MODE; command.mode = RobotMode::BLUETOOTH; }
  else if (line == "M1") { command.type = CommandType::SET_MODE; command.mode = RobotMode::AUTONOMOUS; }
  else if (line == "M2") { command.type = CommandType::SET_MODE; command.mode = RobotMode::AI; }
  else if (line == "LED ON" || line == "LED OFF") {
    command.type = CommandType::SET_LED;
    command.enabled = line.endsWith("ON");
  } else if (line == "STATUS") command.type = CommandType::STATUS;
  else if (line == "HELP") command.type = CommandType::HELP;
  else if (line.startsWith("V") && line.length() > 1) {
    const String valueText = line.substring(1);
    bool valid = true;
    for (size_t i = 0; i < valueText.length(); ++i) valid = valid && isDigit(valueText.charAt(i));
    const long value = valueText.toInt();
    if (valid && value >= 0 && value <= 255) {
      command.type = CommandType::SET_SPEED;
      command.speed = static_cast<uint8_t>(value > Config::MAX_SPEED ? Config::MAX_SPEED : value);
    }
  }
  if (command.type == CommandType::INVALID) command.error = "INVALID_COMMAND";
  return command;
}

bool BluetoothController::poll(ParsedCommand& command) {
  while (serialBt_.available()) {
    const char c = static_cast<char>(serialBt_.read());
    lastReceiveMs_ = millis();
    // Les applications serie envoient souvent une touche sans retour ligne.
    if ((c == '\n' || c == '\r') && buffer_.length() > 0) {
      command = overflow_ ? ParsedCommand{} : parseBluetooth(buffer_);
      if (overflow_) command.error = "COMMAND_TOO_LONG";
      buffer_ = "";
      overflow_ = false;
      return true;
    }
    if (c == '\n' || c == '\r') continue;
    if (buffer_.length() < Config::MAX_COMMAND_LENGTH) buffer_ += c;
    else overflow_ = true;
  }
  return false;
}

bool BluetoothController::connected() { return serialBt_.hasClient(); }
uint32_t BluetoothController::lastReceiveMs() const { return lastReceiveMs_; }
void BluetoothController::sendLine(const String& line) { if (connected()) serialBt_.println(line); }
void BluetoothController::clearInput() { buffer_ = ""; overflow_ = false; while (serialBt_.available()) serialBt_.read(); }

void BluetoothController::sendHelp() {
  sendLine("F/B/L/R/Q/E: mouvement; S: arret; STOP: urgence; RESET: rearmement");
  sendLine("M0: Bluetooth; M1: autonome; M2: IA; V0..255; LED ON/OFF; STATUS; HELP");
}
