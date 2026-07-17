#pragma once

#include <Arduino.h>
#include "config.h"
#include "robot_types.h"

class SerialProtocol {
 public:
  explicit SerialProtocol(Stream& stream);
  bool poll(ParsedCommand& command);
  void clearInput();
  uint32_t lastReceiveMs() const;
  void sendLine(const String& line);
  void sendError(const String& code);
  void sendDistance(const char* side, float cm);
  static ParsedCommand parseLine(String line, CommandSource source);

 private:
  Stream& stream_;
  String buffer_;
  bool overflow_ = false;
  uint32_t lastReceiveMs_ = 0;
};
