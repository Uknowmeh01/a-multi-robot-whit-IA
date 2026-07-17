#pragma once

#include <Arduino.h>

namespace Config {

// IMPORTANT : valeurs de reference pour ESP32 DevKit V1. Verifier le cablage
// avant d'alimenter. Les ECHO HC-SR04 (5 V) exigent des diviseurs vers 3,3 V.
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr char BLUETOOTH_NAME[] = "AI-Robot-Rover";

// Deux TB6612FNG. Relier physiquement les deux broches STBY ensemble.
constexpr uint8_t MOTOR_STBY_PIN = 4;
constexpr uint8_t FL_IN1_PIN = 13;
constexpr uint8_t FL_IN2_PIN = 14;
constexpr uint8_t FL_PWM_PIN = 25;
constexpr uint8_t RL_IN1_PIN = 26;
constexpr uint8_t RL_IN2_PIN = 27;
constexpr uint8_t RL_PWM_PIN = 33;
constexpr uint8_t FR_IN1_PIN = 16;
constexpr uint8_t FR_IN2_PIN = 17;
constexpr uint8_t FR_PWM_PIN = 18;
constexpr uint8_t RR_IN1_PIN = 19;
constexpr uint8_t RR_IN2_PIN = 23;
constexpr uint8_t RR_PWM_PIN = 5;

constexpr bool FL_INVERTED = false;
constexpr bool RL_INVERTED = false;
constexpr bool FR_INVERTED = true;
constexpr bool RR_INVERTED = true;

constexpr uint8_t PWM_CHANNEL_FL = 0;
constexpr uint8_t PWM_CHANNEL_RL = 1;
constexpr uint8_t PWM_CHANNEL_FR = 2;
constexpr uint8_t PWM_CHANNEL_RR = 3;
constexpr uint32_t PWM_FREQUENCY = 18000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8;
constexpr uint8_t MAX_SPEED = 210;
constexpr uint8_t DEFAULT_SPEED = 130;
constexpr uint8_t MIN_TEST_SPEED = 55;
constexpr uint8_t AUTONOMOUS_SPEED = 125;
constexpr uint8_t TURN_SPEED = 115;
constexpr uint8_t REVERSE_SPEED = 95;
constexpr uint8_t ACCEL_STEP = 6;
constexpr uint8_t DECEL_STEP = 10;
constexpr uint32_t MOTOR_RAMP_INTERVAL_MS = 20;
constexpr uint32_t COMMAND_TIMEOUT_MS = 700;
constexpr uint32_t SERIAL_LINK_TIMEOUT_MS = 2500;
constexpr uint32_t BLUETOOTH_LINK_TIMEOUT_MS = 1200;

// GPIO 2, 12 et 15 sont des broches de strapping sur de nombreuses cartes.
// Les TRIG ne doivent jamais les tirer au niveau oppose pendant le demarrage.
constexpr uint8_t US_LEFT_TRIG_PIN = 2;
constexpr uint8_t US_LEFT_ECHO_PIN = 34;
constexpr uint8_t US_CENTER_TRIG_PIN = 15;
constexpr uint8_t US_CENTER_ECHO_PIN = 35;
constexpr uint8_t US_RIGHT_TRIG_PIN = 12;
constexpr uint8_t US_RIGHT_ECHO_PIN = 39;
constexpr uint32_t ULTRASONIC_TIMEOUT_US = 24000;
constexpr uint32_t ULTRASONIC_SENSOR_SPACING_MS = 35;
constexpr uint8_t ULTRASONIC_MEDIAN_SAMPLES = 5;
constexpr float ULTRASONIC_MIN_VALID_CM = 2.0F;
constexpr float ULTRASONIC_MAX_VALID_CM = 400.0F;
constexpr float SLOW_DISTANCE_CM = 55.0F;
constexpr float MIN_DISTANCE_CM = 28.0F;
constexpr float EMERGENCY_DISTANCE_CM = 12.0F;
constexpr uint8_t SENSOR_MAX_CONSECUTIVE_FAILURES = 8;

constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;
constexpr uint8_t BH1750_ADDRESS = 0x23;
constexpr uint32_t LIGHT_READ_INTERVAL_MS = 300;
constexpr float DARK_THRESHOLD_LUX = 35.0F;

constexpr uint8_t SOUND_ADC_PIN = 36;
constexpr uint16_t SOUND_STRONG_THRESHOLD = 2850;
constexpr float SOUND_FILTER_ALPHA = 0.12F;
constexpr uint32_t SOUND_SAMPLE_INTERVAL_MS = 5;
constexpr uint32_t CLAP_DEBOUNCE_MS = 900;

// Bande ou anneau WS2812 : plusieurs LEDs pilotees avec une seule broche.
constexpr uint8_t LED_DATA_PIN = 32;
constexpr uint16_t LED_COUNT = 4;
constexpr uint8_t LED_BRIGHTNESS = 60;
constexpr uint32_t LED_BLINK_INTERVAL_MS = 350;

constexpr uint32_t TELEMETRY_INTERVAL_MS = 250;
constexpr uint32_t STATUS_INTERVAL_MS = 1000;
constexpr uint32_t AUTONOMOUS_REVERSE_MS = 550;
constexpr uint32_t AUTONOMOUS_TURN_MS = 650;
constexpr uint32_t AUTONOMOUS_STUCK_TIMEOUT_MS = 7000;
constexpr uint8_t MAX_COMMAND_LENGTH = 96;

}  // namespace Config
