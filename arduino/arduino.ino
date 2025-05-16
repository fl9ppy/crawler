#include <AFMotor.h>
#include <Servo.h>

// Motor connections (M1 and M2 on shield)
AF_DCMotor motor1(1); // Left motor
AF_DCMotor motor2(2); // Right motor

// Servos on shield (D9 and D10)
Servo servoPan;
Servo servoTilt;

// Ultrasonic sensor
const int TRIG_PIN = 6;
const int ECHO_PIN = 7;

void setup() {
  Serial.begin(9600);

  // Motor setup
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  stopMotors();

  // Attach servos on shield
  servoPan.attach(9);  // Servo1 on shield
  servoTilt.attach(10); // Servo2 on shield

  // Ultrasonic setup
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

// Motor control
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

// Ultrasonic distance
float readDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

