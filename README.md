Crawler
=======

Crawler is a remote-controlled and semi-autonomous robot designed for exploration, research, and terrain navigation.
The system is split between a Raspberry Pi (for high-level tasks like facial detection and remote control)
and an Arduino Uno (handling real-time motor and sensor operations).

Features
--------
- Dual-mode control: remote (via web) or autonomous
- Live camera feed with face detection overlays
- Keyboard-based movement (WASD + Space)
- Pan/tilt camera control via buttons
- Distance sensing using an ultrasonic sensor
- Modular design with YAML-based configuration
- Dynamic Arduino pin mapping via serial

System Architecture
-------------------

Raspberry Pi 4B:
- Hosts Flask web interface
- Streams camera feed with OpenCV
- Sends commands to Arduino via serial
- Parses and applies settings.yaml

Arduino Uno + OSEPP Motor/Servo Shield:
- Controls 2 DC motors (M1/M2 terminals)
- Controls 2 servos (Servo1 = D9, Servo2 = D10)
- Reads distance from ultrasonic sensor
- Responds to serial commands from Pi

Folder Structure
----------------

crawler/
├── arduino/
│   └── crawler.ino
├── pi/
│   ├── control/
│   │   └── robot_serial.py
│   ├── vision/
│   │   └── camera.py
│   ├── web/
│   │   ├── remote.py
│   │   └── templates/
│   │       └── index.html
│   ├── config/
│   │   └── settings.yaml
│   └── main.py
├── boot/
│   └── startup.service
├── requirements.txt
└── README.txt

Setup Instructions
------------------

Arduino:
1. Flash arduino/crawler.ino to your Arduino Uno using Arduino IDE or arduino-cli
2. Make sure the AFMotor and Servo libraries are installed
3. Connect motors, servos, and ultrasonic sensor as defined in settings.yaml

Raspberry Pi:
1. Clone this repo to your Pi
2. Run: pip install -r requirements.txt
3. Adjust config/settings.yaml as needed
4. Run: python3 main.py
5. Visit: http://<your-pi-ip>:5000

Autostart on Boot:
1. Copy boot/startup.service to /etc/systemd/system/
2. Run:
   sudo systemctl daemon-reload
   sudo systemctl enable startup.service
   sudo systemctl start startup.service
