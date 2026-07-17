#include <Arduino.h>
#include "bluetooth_controller.h"
#include "config.h"

BluetoothController bluetooth;

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  Serial.printf("Bluetooth %s\n", bluetooth.begin() ? "OK" : "ECHEC");
  Serial.printf("Associer le telephone a %s et envoyer HELP. Aucun moteur n'est pilote par ce test.\n", Config::BLUETOOTH_NAME);
}

void loop() {
  ParsedCommand command;
  if (bluetooth.poll(command)) {
    Serial.printf("Commande validee: type=%u motion=%u speed=%u error=%s\n",
                  static_cast<unsigned>(command.type), static_cast<unsigned>(command.motion),
                  command.speed, command.error.c_str());
    if (command.type == CommandType::HELP) bluetooth.sendHelp();
    else bluetooth.sendLine(command.type == CommandType::INVALID ? "ERROR,INVALID_COMMAND" : "OK");
  }
}
