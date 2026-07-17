# AI-Powered Four-Wheel Rover

## About the Project

This project is a four-wheel mobile robot designed to move around, detect obstacles, react to sound and light, communicate through Bluetooth, and use a camera with artificial intelligence.

The idea started because I was bored and wanted to build something real instead of just watching videos or starting another project that I would forget about after a few days.

At first, I only wanted to make a simple robot car. Then I started adding more ideas, such as obstacle detection, Bluetooth control, a camera, sound detection, light sensing, and artificial intelligence.

The goal is to build a robot that can work in both manual and autonomous modes. It should also be modular, so I can improve it over time without rebuilding everything from the beginning.

## Main Goals

The robot should be able to:

* move forward and backward;
* turn left and right;
* detect obstacles in front and on both sides;
* avoid obstacles automatically;
* connect to a phone or computer through Bluetooth;
* react to light levels;
* detect sounds;
* display its status using LEDs;
* use a camera to observe its surroundings;
* recognize simple objects using artificial intelligence;
* switch between manual and autonomous control.

## Why I Chose This Project

I started this project because I was bored and wanted to challenge myself with something that combined several things I was interested in.

I wanted to create something physical that I could actually test, improve, and control. Building a robot seemed like a good way to learn programming, electronics, 3D design, and artificial intelligence at the same time.

The project became more serious as I worked on it. What started as a small idea slowly turned into a complete rover with several sensors, a camera, Bluetooth control, and its own AI system.

## Artificial Intelligence

The artificial intelligence part has taken the most time.

Making the robot move is not the hardest part. The real challenge is making it understand what it sees and react correctly.

For the AI system, I need to:

* capture images from the camera;
* process those images on the Raspberry Pi;
* use a lightweight object-detection model;
* keep the detection fast enough for a moving robot;
* send movement decisions to the ESP32;
* combine camera information with data from the other sensors.

The first version of the AI will not be extremely advanced. The main goal is for the robot to recognize a few objects and make simple decisions based on what it sees.

For example, the robot could detect an obstacle, identify an object, stop, or choose another direction.

This part requires a lot of testing because the Raspberry Pi must process the camera image while the robot is still moving and reading sensor data.

## How the Robot Works

The robot will use two main control boards.

### Raspberry Pi

The Raspberry Pi will handle:

* the camera;
* image processing;
* object detection;
* artificial intelligence;
* high-level decisions;
* communication with the ESP32.

### ESP32

The ESP32 will handle:

* motor control;
* ultrasonic sensors;
* the light sensor;
* the sound sensor;
* the LEDs;
* Bluetooth communication;
* fast hardware responses.

The Raspberry Pi acts as the main brain, while the ESP32 controls the hardware and reacts quickly to sensor information.

## Planned Dimensions

The approximate dimensions are:

* length: 280 mm;
* width: 200 mm;
* maximum height: 150 mm;
* wheel diameter: between 65 and 80 mm;
* ground clearance: between 15 and 25 mm.

The chassis will have two levels.

The lower level will contain the motors, battery, motor drivers, power system, and most of the wiring.

The upper level will contain the Raspberry Pi, ESP32, camera, sensors, and electronic modules.

## Required Parts

### Main Control Boards

* 1 Raspberry Pi 4 with 4 GB of RAM;
* 1 ESP32 DevKit;
* 1 microSD card with 32 or 64 GB of storage;
* 1 microSD card reader;
* 1 Raspberry Pi cooling fan or heatsink kit.

### Camera and Artificial Intelligence

* 1 Raspberry Pi Camera Module 3 Wide;
* 1 compatible CSI ribbon cable;
* 1 adjustable camera mount;
* Python;
* OpenCV;
* TensorFlow Lite or another lightweight AI framework;
* a lightweight object-detection model.

### Motors and Movement

* 4 DC geared motors;
* 4 wheels between 65 and 80 mm;
* 2 TB6612FNG dual motor drivers;
* 4 motor mounts;
* 1 two-level chassis;
* M3 screws, nuts, washers, and spacers.

### Sensors

* 3 HC-SR04 ultrasonic sensors;
* 1 BH1750 light sensor;
* 1 analog microphone or sound-detection module;
* 1 logic-level converter from 5 V to 3.3 V;
* resistors for voltage dividers;
* jumper wires.

### Bluetooth

Bluetooth will mainly be handled by the ESP32 because it already includes Bluetooth support.

It will be used to:

* control the robot from a phone;
* send movement commands;
* switch between different modes;
* control the LEDs;
* receive information from the robot;
* stop the robot remotely.

The Raspberry Pi may also use Bluetooth if needed, but the ESP32 will be responsible for the main remote-control system.

### LEDs and Indicators

* 4 to 8 standard or RGB LEDs;
* 220 to 330 ohm resistors;
* an optional WS2812B LED strip or ring;
* an optional active buzzer.

The LEDs can be used to show different states.

For example:

* green for normal operation;
* blue for an active Bluetooth connection;
* orange when an obstacle is detected;
* red for an error or low battery.

### Power System

* 1 protected rechargeable battery pack;
* 1 compatible battery charger;
* 1 voltage converter providing a stable 5.1 V output;
* 1 main power switch;
* 1 fuse;
* 1 battery voltage-monitoring module;
* suitable power connectors;
* electrical wiring.

The battery should be placed low and near the center of the chassis to keep the robot stable.

The motors and Raspberry Pi should receive stable power, and all grounds should be connected correctly.

### Construction and Wiring

* Dupont jumper wires;
* flexible electrical wire;
* JST connectors;
* screw terminals;
* heat-shrink tubing;
* cable ties;
* a small prototyping board;
* M3 screws;
* M3 nuts;
* washers;
* spacers;
* ultrasonic sensor mounts;
* camera mount;
* battery holder;
* wheel guards;
* small screwdrivers;
* a multimeter;
* soldering equipment.

## Sensor Placement

The three ultrasonic sensors will be placed at the front of the robot.

One sensor will face directly forward.

The other two sensors will point slightly to the left and right, at an angle of around 35 degrees.

This should allow the robot to detect obstacles before hitting them and choose a safer direction.

The light sensor will be placed on top of the robot and away from the LEDs so that the robot does not measure its own light.

The microphone will be placed as far as possible from the motors because motor noise and vibration could affect its readings.

The camera will be mounted at the front and slightly higher than the ultrasonic sensors to give it a clear view.

## Control Modes

### Bluetooth Mode

In Bluetooth mode, the robot can be controlled from a phone or computer.

The planned commands are:

* move forward;
* move backward;
* turn left;
* turn right;
* stop;
* control the LEDs;
* change the operating mode.

### Autonomous Mode

In autonomous mode, the robot will use the ultrasonic sensors to avoid obstacles.

If it detects something in front of it, it will stop, compare the distances on the left and right, and choose a safer direction.

### AI Mode

In AI mode, the robot will use the camera and Raspberry Pi to analyze its surroundings.

Depending on the model being used, it may be able to recognize certain objects and react differently to them.

The AI will work together with the ultrasonic sensors because the camera alone may not always give accurate distance information.

## 3D Design

The chassis will be designed for 3D printing.

The design should be:

* modular;
* easy to assemble;
* easy to repair;
* compatible with M3 screws;
* strong enough to hold all components;
* suitable for a standard 3D printer;
* divided into separate printable parts;
* easy to modify later.

The design will include:

* a lower chassis plate;
* an upper electronics plate;
* four motor mounts;
* three ultrasonic sensor mounts;
* an adjustable camera mount;
* a Raspberry Pi mount;
* an ESP32 mount;
* a battery holder;
* motor-driver mounts;
* LED mounts;
* a light-sensor mount;
* a microphone mount;
* wheel protection;
* ventilation openings;
* cable-routing holes.

## Main Challenges

One of the biggest challenges is fitting all the components inside a compact robot without making the wiring difficult to manage.

Other challenges include:

* keeping the Raspberry Pi cool;
* reducing camera vibration;
* reducing noise around the microphone;
* supplying enough power to the motors;
* preventing voltage drops;
* reading several sensors at the same time;
* avoiding interference between ultrasonic sensors;
* maintaining a stable Bluetooth connection;
* running artificial intelligence quickly enough;
* connecting the Raspberry Pi and ESP32 reliably.

The AI system is the most difficult part because it requires the robot to process images, understand the result, and react while everything else is still running.

## Future Improvements

Possible future upgrades include:

* a robotic arm;
* person-following mode;
* a movable camera using servomotors;
* voice commands;
* a custom mobile application;
* better object recognition;
* room mapping;
* GPS;
* a small display;
* automatic charging;
* improved battery monitoring;
* more advanced autonomous navigation.

## Conclusion

This project started because I was bored, but it became a much bigger challenge than I expected.

It gives me the opportunity to learn about programming, electronics, robotics, artificial intelligence, Bluetooth communication, and 3D design in one project.

The first version will probably not work perfectly, and that is part of the process. The goal is to build it, test it, find problems, and improve it step by step.

Even if the robot drives into a wall during the first tests, at least the obstacle-detection system will know what to work on next.

## Author

Created by **[Your name or username]** for Hack Club.
