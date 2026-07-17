# AI Robot Rover 

<img width="1047" height="784" alt="Screenshot 2026-07-17 162009" src="https://github.com/user-attachments/assets/7109efb7-1d62-4a83-8e4f-e09f7bfac228" />

A modular, 3D-printable indoor robot rover built around a Raspberry Pi 4 and an ESP32.

The project combines a parametric mechanical chassis, motor control, environmental sensors, a camera, autonomous behaviors, and a local web interface for controlling the robot from a phone or computer.

> [!IMPORTANT]
> The chassis design and software prototype are complete, but the full robot has not yet been physically assembled or tested.

## Project goals

The long-term goal is to build a small indoor assistant robot capable of:

- manual control from a phone;
- autonomous obstacle avoidance;
- camera-based object detection;
- navigation between rooms using visual markers;
- receiving simple voice commands;
- playing sounds or messages through a speaker;
- carrying small objects or messages around the house.

One planned example is telling the robot to go to the living room or to a family member's room. The robot would follow a known route, recognize a visual marker at its destination, stop, and play a notification sound.

## Hardware architecture

The robot uses two main computers:

### Raspberry Pi 4

The Raspberry Pi handles high-level functions:

- camera processing;
- object detection;
- web server and control interface;
- navigation logic;
- communication with the ESP32;
- future voice recognition and audio playback.

### ESP32

The ESP32 handles real-time hardware control:

- four DC motors;
- two TB6612FNG motor drivers;
- ultrasonic distance sensors;
- LEDs;
- environmental sensors;
- emergency stopping;
- serial communication with the Raspberry Pi.

## Planned hardware

- Raspberry Pi 4
- ESP32 development board
- 4 DC geared motors
- 2 TB6612FNG motor drivers
- Raspberry Pi Camera Module 3 Wide
- HC-SR04 ultrasonic sensors
- BH1750 light sensor
- microphone
- LEDs
- battery and power regulation system
- future USB speaker
- 3D-printed mechanical chassis
- M3 screws, nuts, and threaded inserts

Exact motor, battery, and mounting dimensions may still be adjusted to match the purchased hardware.

## Mechanical design

The chassis was designed as a parametric, multi-part KCL project in Zoo Design Studio.

### Main dimensions

| Property | Value |
|---|---:|
| Main deck size | 280 × 200 mm |
| Mechanical stack height | 148 mm |
| Approximate maximum length with angled inserts | 304 mm |
| M3 clearance hole | 3.4 mm |
| Maximum individual print area | 220 × 220 mm |

All individual parts are designed to fit on a standard 220 × 220 mm print bed.

### Mechanical features

- split lower deck for easier printing;
- three alignment tongues between lower-deck sections;
- aligned M3 joint holes;
- removable front and rear panels;
- removable corner brackets;
- reserved mounting areas for motors, battery, and motor drivers;
- cable-routing openings;
- separate sensor and camera inserts;
- modular upper electronics plate;
- removable top cover;
- editable shared parameters;
- exploded and inspection assemblies.

The current model intentionally contains only mechanical geometry and empty mounting provisions. Electronics, motors, wheels, batteries, cameras, sensors, and cables are not modeled as solid components.

## CAD files

The main CAD files include:

```text
cad/
├── main.kcl
├── explodedView.kcl
├── lowerDeckAssembly.kcl
├── parameters.kcl
├── chassisDimensions.md
├── removableCornerBracket.kcl
└── printable-parts/
```

The exact directory structure may differ depending on how the exported Zoo project is organized.

### Main CAD documents

- `main.kcl` contains the complete assembled chassis.
- `explodedView.kcl` contains the exploded assembly.
- `lowerDeckAssembly.kcl` provides a clear view of the split-deck joint.
- `parameters.kcl` contains shared editable dimensions.
- `chassisDimensions.md` documents dimensions, quantities, print envelopes, and export information.

### Printable parts

The project contains separate KCL parts for:

- front lower plate;
- rear lower plate;
- upper electronics plate;
- top cover;
- front sensor panel;
- rear utility panel;
- ultrasonic mounting insert;
- camera mounting insert;
- lower spacer posts;
- upper spacer posts;
- removable corner brackets.

STEP and STL binaries are not currently included because the original design environment exports KCL source files. Each printable part is isolated and ready to be exported from Zoo Design Studio.

## Software

The software is divided between the ESP32 and Raspberry Pi.

```text
software/
├── esp32/
└── raspberry-pi/
```

### ESP32 software

The ESP32 firmware is responsible for:

- motor control;
- serial command parsing;
- ultrasonic sensor measurements;
- lighting control;
- Bluetooth communication;
- autonomous safety behavior.

### Raspberry Pi software

The Raspberry Pi application includes:

- robot control logic;
- camera support;
- object-detection prototype;
- serial communication with the ESP32;
- operating-mode management;
- local web server;
- phone-friendly control interface;
- installation script;
- systemd service;
- tests.

## Web control interface

The Raspberry Pi hosts a local control website.

The interface is intended to provide:

- movement controls;
- speed adjustment;
- operating-mode selection;
- emergency stop;
- sensor information;
- connection status;
- AI detection information.

When the robot is connected to the same Wi-Fi network as a phone or computer, the interface can be opened using the Raspberry Pi's local IP address.

Example:

```text
http://192.168.1.42:5000
```

The exact IP address depends on the local network.

## Operating modes

The planned modes include:

### Manual mode

The user controls movement from the web interface or Bluetooth.

### Autonomous mode

The robot uses its distance sensors to move while avoiding obstacles.

### AI vision mode

The Raspberry Pi camera runs an object-detection prototype. The current software may use a person as its default target, but this behavior can be changed.

### Future navigation mode

A future version will use visual markers such as AprilTags or ArUco markers.

Each room or waypoint can have a unique marker. The robot will use these markers to follow routes and recognize when it has reached a destination.

## Current project status

### Completed

- [x] Parametric chassis design
- [x] Separate printable parts
- [x] Main mechanical assembly
- [x] Exploded assembly
- [x] Split lower-deck system
- [x] Removable corner brackets
- [x] Motor and battery reserved zones
- [x] ESP32 firmware prototype
- [x] Raspberry Pi software prototype
- [x] Local web interface prototype
- [x] Installation script and service files

### Not yet completed

- [ ] Print physical chassis parts
- [ ] Purchase and measure final hardware
- [ ] Assemble the complete rover
- [ ] Test motor direction and speed
- [ ] Test battery and power distribution
- [ ] Validate sensor measurements
- [ ] Validate emergency-stop behavior
- [ ] Test the Raspberry Pi and ESP32 protocol
- [ ] Test the web interface on the physical robot
- [ ] Add visual-marker navigation
- [ ] Add voice commands
- [ ] Add speaker notifications
- [ ] Document real-world test results

## Safety

Initial motor tests should be performed with the wheels raised off the ground.

Before autonomous testing:

- verify the emergency stop;
- use a low movement speed;
- keep cables away from wheels;
- securely mount the battery;
- add suitable power protection and a fuse;
- test in an open indoor area;
- keep the robot away from stairs and fragile objects.

## Repository structure

A possible repository layout is:

```text
AI-Robot-Rover/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   └── images/
│       └── chassis-final.png
├── cad/
│   ├── main.kcl
│   ├── explodedView.kcl
│   ├── lowerDeckAssembly.kcl
│   ├── parameters.kcl
│   └── chassisDimensions.md
└── software/
    ├── esp32/
    └── raspberry-pi/
```

## Development notes

Some parts of the initial CAD and software were created with assistance from AI development tools.

All generated work will be reviewed, adapted, physically tested, and documented during the construction process. A generated design passing software validation does not guarantee that every mechanical tolerance or hardware integration detail will work correctly on the first physical build.

## Future ideas

- indoor room-to-room navigation;
- voice activation;
- destination markers;
- speaker announcements;
- automatic charging station;
- motor encoders;
- IMU-assisted movement;
- battery monitoring;
- removable payload platform;
- improved camera mount;
- physical emergency-stop button.

## License

A license has not yet been selected.

Before other people reuse the project, an open-source license such as the MIT License, Apache-2.0, or CERN Open Hardware Licence should be added.

## Author

Created by **Uknowmeh01** as a personal robotics and engineering project.

Built for learning, experimenting, and sharing with the Hack Club community. 

