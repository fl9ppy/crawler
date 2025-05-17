#include <AFMotor.h>
#include <Servo.h>

Servo servoPan;
Servo servoTilt;

int TRIG_PIN = 10;
int ECHO_PIN = 9;
int PAN_PIN = 3;
int TILT_PIN = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("Crawler Arduino Online (Servo Only)");

  servoPan.attach(PAN_PIN);
  servoTilt.attach(TILT_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void parseConfig(String command) {
  int trigIndex = command.indexOf("TRIG=");
  int echoIndex = command.indexOf("ECHO=");
  int panIndex = command.indexOf("PAN=");
  int tiltIndex = command.indexOf("TILT=");

  if (trigIndex != -1 && echoIndex != -1) {
    TRIG_PIN = command.substring(trigIndex + 5, command.indexOf(' ', trigIndex)).toInt();
    ECHO_PIN = command.substring(echoIndex + 5, command.indexOf(' ', echoIndex)).toInt();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
  }

  if (panIndex != -1 && tiltIndex != -1) {
    PAN_PIN = command.substring(panIndex + 4, command.indexOf(' ', panIndex)).toInt();
    TILT_PIN = command.substring(tiltIndex + 5).toInt();
    servoPan.attach(PAN_PIN);
    servoTilt.attach(TILT_PIN);
  }
}

void loop() {
  if (Serial.available()) {
    String command = "";
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') break;
      if (c >= 32 && c <= 126) command += c;
    }

    command.replace("\r", "");
    command.trim();

    if (command.startsWith("SERVO")) {
      int pan = command.substring(6, command.indexOf(' ', 6)).toInt();
      int tilt = command.substring(command.indexOf(' ', 6) + 1).toInt();
      servoPan.write(pan);
      servoTilt.write(tilt);
    } else if (command == "PING") {
      float dist = readDistance();
      Serial.print("DIST ");
      Serial.println(dist);
    } else if (command.startsWith("CONFIG")) {
      parseConfig(command);
    } else {
      Serial.print("UNKNOWN: ");
      Serial.println(command);
    }
  }
}

