#pragma once

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "config.h"
#include "robot_types.h"

class BluetoothController {
 public:
  bool begin();
  bool poll(ParsedCommand& command);
  bool connected();
  uint32_t lastReceiveMs() const;
  void clearInput();
  void sendLine(const String& line);
  void sendHelp();

 private:
  BluetoothSerial serialBt_;
  String buffer_;
  uint32_t lastReceiveMs_ = 0;
  bool overflow_ = false;
  ParsedCommand parseBluetooth(String line) const;
};
