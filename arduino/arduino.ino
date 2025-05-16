#include <AFMotor.h>
#include <Servo.h>

// DC motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// Servos
Servo servoPan;
Servo servoTilt;

// Pins (configurable via serial)
int TRIG_PIN = 6;
int ECHO_PIN = 7;
int PAN_PIN = 9;
int TILT_PIN = 10;

void setup() {
  Serial.begin(9600);

  motor1.setSpeed(200);
  motor2.setSpeed(200);
  stopMotors();

  Serial.println("ARDUINO READY");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "FWD") moveForward();
    else if (command == "BACK") moveBackward();
    else if (command == "LEFT") turnLeft();
    else if (command == "RIGHT") turnRight();
    else if (command == "STOP") stopMotors();
    else if (command.startsWith("SERVO")) {
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
    }
  }
}

// CONFIG TRIG=6 ECHO=7 PAN=9 TILT=10
void parseConfig(String cmd) {
  int trig = cmd.indexOf("TRIG=");
  int echo = cmd.indexOf("ECHO=");
  int pan  = cmd.indexOf("PAN=");
  int tilt = cmd.indexOf("TILT=");

  if (trig != -1) TRIG_PIN = cmd.substring(trig + 5, cmd.indexOf(" ", trig + 5)).toInt();
  if (echo != -1) ECHO_PIN = cmd.substring(echo + 5, cmd.indexOf(" ", echo + 5)).toInt();
  if (pan  != -1) PAN_PIN  = cmd.substring(pan + 4, cmd.indexOf(" ", pan + 4)).toInt();
  if (tilt != -1) TILT_PIN = cmd.substring(tilt + 5).toInt();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoPan.attach(PAN_PIN);
  servoTilt.attach(TILT_PIN);

  Serial.print("CONFIG_OK TRIG=");
  Serial.print(TRIG_PIN);
  Serial.print(" ECHO=");
  Serial.print(ECHO_PIN);
  Serial.print(" PAN=");
  Serial.print(PAN_PIN);
  Serial.print(" TILT=");
  Serial.println(TILT_PIN);
}

void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void moveBackward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

