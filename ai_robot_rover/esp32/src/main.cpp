#include <Arduino.h>

#include "autonomous_controller.h"
#include "bluetooth_controller.h"
#include "config.h"
#include "led_controller.h"
#include "motor_controller.h"
#include "robot_types.h"
#include "sensor_manager.h"
#include "serial_protocol.h"
#include "ultrasonic_manager.h"

MotorController motors;
UltrasonicManager ultrasonics;
SensorManager sensors;
LedController leds;
SerialProtocol serialProtocol(Serial);
BluetoothController bluetooth;
AutonomousController autonomous(motors);

RobotMode currentMode = RobotMode::MANUAL;
uint8_t currentSpeed = Config::DEFAULT_SPEED;
bool emergencyLatched = false;
bool serialTimedOut = false;
bool bluetoothWasConnected = false;
bool sensorFaultWasActive = false;
uint32_t lastMotionCommandMs = 0;
uint32_t lastTelemetryMs = 0;
uint32_t lastStatusMs = 0;

void publishStatus() {
  const auto& d = ultrasonics.readings();
  const auto& s = sensors.readings();
  serialProtocol.sendLine(String("STATUS,MODE=") + modeName(currentMode) +
                          ",SPEED=" + currentSpeed +
                          ",MOVING=" + (motors.isMoving() ? "1" : "0") +
                          ",EMERGENCY=" + (emergencyLatched ? "1" : "0") +
                          ",AUTO_STATE=" + autonomous.stateName());
  if (bluetooth.connected()) {
    bluetooth.sendLine(String("MODE=") + modeName(currentMode) +
                       ", SPEED=" + currentSpeed +
                       ", EMERGENCY=" + (emergencyLatched ? "YES" : "NO") +
                       ", L/C/R=" + String(d.left, 1) + "/" + String(d.center, 1) + "/" + String(d.right, 1) +
                       ", LUX=" + String(s.lightLux, 1) + ", SOUND=" + String(static_cast<int>(s.soundAverage)));
  }
}

void changeMode(RobotMode newMode, CommandSource source) {
  motors.stopAll(true);
  autonomous.stop();
  serialProtocol.clearInput();
  bluetooth.clearInput();
  currentMode = newMode;
  leds.setMode(currentMode);
  lastMotionCommandMs = millis();
  if (currentMode == RobotMode::AUTONOMOUS && !emergencyLatched) autonomous.start();
  serialProtocol.sendLine(String("MODE,") + modeName(currentMode));
  if (bluetooth.connected()) bluetooth.sendLine(String("MODE,") + modeName(currentMode));
  serialProtocol.sendLine(String("EVENT,MODE_CHANGED,") +
                          (source == CommandSource::SERIAL_LINK ? "SERIAL" : "BLUETOOTH"));
}

bool sourceCanMove(CommandSource source) {
  if (source == CommandSource::BLUETOOTH_LINK) return currentMode == RobotMode::BLUETOOTH;
  return currentMode == RobotMode::MANUAL || currentMode == RobotMode::AI;
}

void latchEmergency(const String& reason) {
  if (emergencyLatched) return;
  emergencyLatched = true;
  motors.emergencyStop();
  autonomous.emergencyStop();
  leds.setEmergency(true);
  serialProtocol.sendLine(String("EMERGENCY_STOP,") + reason);
  bluetooth.sendLine(String("EMERGENCY_STOP,") + reason);
}

void handleCommand(const ParsedCommand& command) {
  if (command.type == CommandType::INVALID) {
    if (command.source == CommandSource::SERIAL_LINK) serialProtocol.sendError(command.error);
    else bluetooth.sendLine(String("ERROR,") + command.error);
    return;
  }
  if (command.type == CommandType::EMERGENCY_STOP) {
    latchEmergency(command.source == CommandSource::SERIAL_LINK ? "SERIAL_COMMAND" : "BLUETOOTH_COMMAND");
    return;
  }
  if (command.type == CommandType::CLEAR_EMERGENCY) {
    const auto& distances = ultrasonics.readings();
    const bool distancesValid = distances.leftHealthy && distances.centerHealthy && distances.rightHealthy &&
                                isfinite(distances.left) && isfinite(distances.center) && isfinite(distances.right);
    if (!distancesValid || ultrasonics.emergencyObstacle()) {
      const char* error = distancesValid ? "OBSTACLE_TOO_CLOSE" : "SENSOR_UNAVAILABLE";
      if (command.source == CommandSource::SERIAL_LINK) serialProtocol.sendError(error);
      else bluetooth.sendLine(String("ERROR,") + error);
      return;
    }
    emergencyLatched = false;
    motors.stopAll(true);
    leds.setEmergency(false);
    changeMode(RobotMode::MANUAL, command.source);
    serialProtocol.sendLine("EMERGENCY_CLEARED");
    bluetooth.sendLine("EMERGENCY_CLEARED");
    return;
  }
  if (command.type == CommandType::STATUS) { publishStatus(); return; }
  if (command.type == CommandType::HELP) { bluetooth.sendHelp(); return; }
  if (command.type == CommandType::STOP) {
    motors.stopAll(true);
    lastMotionCommandMs = millis();
    return;
  }
  if (emergencyLatched) {
    if (command.source == CommandSource::SERIAL_LINK) serialProtocol.sendError("EMERGENCY_LATCHED");
    else bluetooth.sendLine("ERROR,EMERGENCY_LATCHED");
    return;
  }
  if (command.type == CommandType::SET_MODE) {
    changeMode(command.mode, command.source);
  } else if (command.type == CommandType::SET_SPEED) {
    currentSpeed = min(command.speed, Config::MAX_SPEED);
    serialProtocol.sendLine(String("SPEED,") + currentSpeed);
  } else if (command.type == CommandType::SET_LED) {
    leds.setUserEnabled(command.enabled);
    serialProtocol.sendLine(String("LED,") + (command.enabled ? "ON" : "OFF"));
  } else if (command.type == CommandType::MOVE) {
    if (!sourceCanMove(command.source)) {
      if (command.source == CommandSource::SERIAL_LINK) serialProtocol.sendError("SOURCE_NOT_OWNER");
      else bluetooth.sendLine("ERROR,SOURCE_NOT_OWNER; SEND M0 FIRST");
      return;
    }
    const uint8_t speed = command.source == CommandSource::BLUETOOTH_LINK
                              ? currentSpeed
                              : min(command.speed, Config::MAX_SPEED);
    motors.setMotion(command.motion, speed);
    lastMotionCommandMs = millis();
  }
}

void publishTelemetry() {
  const auto& d = ultrasonics.readings();
  const auto& s = sensors.readings();
  serialProtocol.sendDistance("LEFT", d.left);
  serialProtocol.sendDistance("CENTER", d.center);
  serialProtocol.sendDistance("RIGHT", d.right);
  if (isfinite(s.lightLux)) Serial.printf("LIGHT,%.1f\n", s.lightLux);
  Serial.printf("SOUND,%u\n", static_cast<unsigned>(s.soundAverage));
  if (ultrasonics.closeObstacle()) serialProtocol.sendLine("OBSTACLE,CENTER");
}

void setup() {
  Serial.begin(Config::SERIAL_BAUD);
  motors.begin();
  ultrasonics.begin();
  sensors.begin();
  leds.begin();
  leds.setMode(currentMode);
  const bool bluetoothOk = bluetooth.begin();
  motors.emergencyStop();
  delay(30);
  serialProtocol.sendLine("READY,AI_ROBOT_ROVER");
  if (!bluetoothOk) serialProtocol.sendError("BLUETOOTH_INIT_FAILED");
  if (!sensors.readings().lightHealthy) serialProtocol.sendError("BH1750_INIT_FAILED");
}

void loop() {
  ParsedCommand command;
  while (serialProtocol.poll(command)) {
    serialTimedOut = false;
    handleCommand(command);
  }
  while (bluetooth.poll(command)) handleCommand(command);

  ultrasonics.update();
  sensors.update();
  const auto& sensorData = sensors.readings();
  leds.setDark(sensorData.lightHealthy && isfinite(sensorData.lightLux) &&
               sensorData.lightLux < Config::DARK_THRESHOLD_LUX);
  if (sensors.consumeClapEvent()) {
    leds.setUserEnabled(!leds.userEnabled());
    serialProtocol.sendLine("EVENT,CLAP,LED_TOGGLED");
  }

  if (ultrasonics.emergencyObstacle()) latchEmergency("ULTRASONIC");
  leds.setObstacle(ultrasonics.closeObstacle());
  const bool sensorFault = ultrasonics.sensorFault();
  leds.setError(sensorFault || !sensorData.lightHealthy);
  if (sensorFault && !sensorFaultWasActive) serialProtocol.sendError("SENSOR_TIMEOUT");
  if (!sensorFault && sensorFaultWasActive) serialProtocol.sendLine("EVENT,SENSORS_RECOVERED");
  sensorFaultWasActive = sensorFault;

  const uint32_t now = millis();
  const bool serialLinkLost = serialProtocol.lastReceiveMs() != 0 &&
                              now - serialProtocol.lastReceiveMs() > Config::SERIAL_LINK_TIMEOUT_MS;
  if (serialLinkLost && currentMode != RobotMode::BLUETOOTH && !serialTimedOut) {
    motors.stopAll(true);
    autonomous.stop();
    serialTimedOut = true;
    if (currentMode != RobotMode::MANUAL) changeMode(RobotMode::MANUAL, CommandSource::SERIAL_LINK);
    serialProtocol.sendError("SERIAL_LINK_TIMEOUT");
  }

  if (!emergencyLatched && !serialTimedOut && currentMode == RobotMode::AUTONOMOUS) {
    autonomous.update(ultrasonics.readings());
  }

  if (!emergencyLatched && motors.isMoving() &&
      currentMode != RobotMode::AUTONOMOUS &&
      now - lastMotionCommandMs > Config::COMMAND_TIMEOUT_MS) {
    motors.stopAll(true);
    serialProtocol.sendLine("EVENT,COMMAND_TIMEOUT,STOPPED");
  }

  const bool btConnected = bluetooth.connected();
  if (bluetoothWasConnected && !btConnected && currentMode == RobotMode::BLUETOOTH) {
    motors.stopAll(true);
    serialProtocol.sendLine("BLUETOOTH,DISCONNECTED");
  } else if (!bluetoothWasConnected && btConnected) {
    serialProtocol.sendLine("BLUETOOTH,CONNECTED");
  }
  bluetoothWasConnected = btConnected;
  if (currentMode == RobotMode::BLUETOOTH && motors.isMoving() &&
      now - bluetooth.lastReceiveMs() > Config::BLUETOOTH_LINK_TIMEOUT_MS) {
    motors.stopAll(true);
    bluetooth.sendLine("EVENT,COMMAND_TIMEOUT,STOPPED");
  }

  if (now - lastTelemetryMs >= Config::TELEMETRY_INTERVAL_MS) {
    lastTelemetryMs = now;
    publishTelemetry();
  }
  if (now - lastStatusMs >= Config::STATUS_INTERVAL_MS) {
    lastStatusMs = now;
    publishStatus();
  }
  leds.setConnected(btConnected || now - serialProtocol.lastReceiveMs() < Config::SERIAL_LINK_TIMEOUT_MS);
  motors.update();
  leds.update();
  yield();
}
