#include <Arduino.h>
#include "config.h"
#include "serial_protocol.h"

SerialProtocol protocol(Serial);

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  Serial.println("Envoyer des lignes du protocole. Ce test ne pilote aucun moteur.");
}

void loop() {
  ParsedCommand command;
  if (protocol.poll(command)) {
    Serial.printf("PARSED,type=%u,motion=%u,mode=%u,speed=%u,error=%s\n",
                  static_cast<unsigned>(command.type), static_cast<unsigned>(command.motion),
                  static_cast<unsigned>(command.mode), command.speed, command.error.c_str());
  }
}
