#include "serial_protocol.h"

namespace {
uint8_t splitCsv(const String& input, String* parts, uint8_t maxParts) {
  uint8_t count = 0;
  int start = 0;
  while (count < maxParts) {
    const int comma = input.indexOf(',', start);
    if (comma < 0) {
      parts[count++] = input.substring(start);
      break;
    }
    parts[count++] = input.substring(start, comma);
    start = comma + 1;
  }
  for (uint8_t i = 0; i < count; ++i) parts[i].trim();
  return count;
}

bool parseSpeed(const String& text, uint8_t& speed) {
  if (text.length() == 0) return false;
  for (size_t i = 0; i < text.length(); ++i) if (!isDigit(text.charAt(i))) return false;
  const long value = text.toInt();
  if (value < 0 || value > 255) return false;
  speed = static_cast<uint8_t>(value > Config::MAX_SPEED ? Config::MAX_SPEED : value);
  return true;
}

bool parseMode(const String& text, RobotMode& mode) {
  if (text == "MANUAL") mode = RobotMode::MANUAL;
  else if (text == "BLUETOOTH") mode = RobotMode::BLUETOOTH;
  else if (text == "AUTONOMOUS") mode = RobotMode::AUTONOMOUS;
  else if (text == "AI") mode = RobotMode::AI;
  else return false;
  return true;
}

bool parseMotion(const String& text, MotionAction& action) {
  if (text == "FORWARD") action = MotionAction::FORWARD;
  else if (text == "BACKWARD") action = MotionAction::BACKWARD;
  else if (text == "LEFT") action = MotionAction::TURN_LEFT;
  else if (text == "RIGHT") action = MotionAction::TURN_RIGHT;
  else return false;
  return true;
}
}  // namespace

SerialProtocol::SerialProtocol(Stream& stream) : stream_(stream) { buffer_.reserve(Config::MAX_COMMAND_LENGTH); }

ParsedCommand SerialProtocol::parseLine(String line, CommandSource source) {
  ParsedCommand command;
  command.source = source;
  line.trim();
  line.toUpperCase();
  String parts[4];
  const uint8_t count = splitCsv(line, parts, 4);
  if (line == "STOP") command.type = CommandType::STOP;
  else if (line == "EMERGENCY_STOP") command.type = CommandType::EMERGENCY_STOP;
  else if (line == "CLEAR_EMERGENCY") command.type = CommandType::CLEAR_EMERGENCY;
  else if (line == "STATUS" || line == "PING") command.type = CommandType::STATUS;
  else if (line == "HELP") command.type = CommandType::HELP;
  else if (count == 3 && parts[0] == "MOVE") {
    command.type = CommandType::MOVE;
    if (!parseMotion(parts[1], command.motion) ||
        (command.motion != MotionAction::FORWARD && command.motion != MotionAction::BACKWARD) ||
        !parseSpeed(parts[2], command.speed)) command.type = CommandType::INVALID;
  } else if (count == 3 && parts[0] == "TURN") {
    command.type = CommandType::MOVE;
    if (!parseMotion(parts[1], command.motion) ||
        (command.motion != MotionAction::TURN_LEFT && command.motion != MotionAction::TURN_RIGHT) ||
        !parseSpeed(parts[2], command.speed)) command.type = CommandType::INVALID;
  } else if (count == 3 && parts[0] == "ROTATE") {
    command.type = CommandType::MOVE;
    if (parts[1] == "LEFT") command.motion = MotionAction::ROTATE_LEFT;
    else if (parts[1] == "RIGHT") command.motion = MotionAction::ROTATE_RIGHT;
    else command.type = CommandType::INVALID;
    if (!parseSpeed(parts[2], command.speed)) command.type = CommandType::INVALID;
  } else if (count == 2 && parts[0] == "MODE") {
    command.type = CommandType::SET_MODE;
    if (!parseMode(parts[1], command.mode)) command.type = CommandType::INVALID;
  } else if (count == 2 && parts[0] == "SPEED") {
    command.type = CommandType::SET_SPEED;
    if (!parseSpeed(parts[1], command.speed)) command.type = CommandType::INVALID;
  } else if (count == 2 && parts[0] == "LED") {
    command.type = CommandType::SET_LED;
    if (parts[1] == "ON") command.enabled = true;
    else if (parts[1] == "OFF") command.enabled = false;
    else command.type = CommandType::INVALID;
  }
  if (command.type == CommandType::INVALID) command.error = "INVALID_COMMAND";
  return command;
}

bool SerialProtocol::poll(ParsedCommand& command) {
  while (stream_.available()) {
    const char c = static_cast<char>(stream_.read());
    lastReceiveMs_ = millis();
    if (c == '\r') continue;
    if (c == '\n') {
      if (overflow_) {
        command = ParsedCommand{};
        command.error = "COMMAND_TOO_LONG";
      } else if (buffer_.length() > 0) {
        command = parseLine(buffer_, CommandSource::SERIAL_LINK);
      } else {
        continue;
      }
      buffer_ = "";
      overflow_ = false;
      return true;
    }
    if (buffer_.length() < Config::MAX_COMMAND_LENGTH) buffer_ += c;
    else overflow_ = true;
  }
  return false;
}

void SerialProtocol::clearInput() {
  buffer_ = "";
  overflow_ = false;
  while (stream_.available()) stream_.read();
}

uint32_t SerialProtocol::lastReceiveMs() const { return lastReceiveMs_; }
void SerialProtocol::sendLine(const String& line) { stream_.println(line); }
void SerialProtocol::sendError(const String& code) { stream_.println("ERROR," + code); }
void SerialProtocol::sendDistance(const char* side, float cm) {
  if (isfinite(cm)) stream_.printf("DISTANCE,%s,%.1f\n", side, cm);
  else stream_.printf("DISTANCE,%s,NAN\n", side);
}
