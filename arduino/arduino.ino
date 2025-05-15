#include <Servo.h>

// Motor pins
const int MOTOR_A_IN1 = 3;
const int MOTOR_A_IN2 = 2;
const int MOTOR_B_IN1 = 5;
const int MOTOR_B_IN2 = 4;

// Servo pins
const int SERVO_PAN_PIN = 9;
const int SERVO_TILT_PIN = 10;

// Ultrasound pins
const int TRIG_PIN = 6;
const int ECHO_PIN = 7;

Servo servoPan, servoTilt;

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);

  servoPan.attach(SERVO_PAN_PIN);
  servoTilt.attach(SERVO_TILT_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("ARDUINO READY");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\\n');
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
    }
  }
}

void moveForward() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}
