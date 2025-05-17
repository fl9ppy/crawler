#include <AFMotor.h>
#include <Servo.h>

// DC motors
AF_DCMotor motor1(1); // M1
AF_DCMotor motor2(2); // M2

// Servos
Servo servoPan;
Servo servoTilt;

// Pins (will be set via CONFIG)
int TRIG_PIN = 6;
int ECHO_PIN = 7;
int PAN_PIN = 9;
int TILT_PIN = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Crawler Arduino Online");

  stopMotors();
}

void loop() {
  static String command = "";
  
  // Build command character by character
  while (Serial.available()) {
    char c = Serial.read();
    
    // Printable characters only
    if (c >= 32 && c <= 126) {
      command += c;
    }

    // Command ends at newline
    if (c == '\n') {
      command.replace("\r", "");  // Strip carriage return just in case
      command.trim();

      Serial.print("CMD RECEIVED: ");
      Serial.println(command);

      // Debug: Print ASCII values
      Serial.print("RAW BYTES: ");
      for (int i = 0; i < command.length(); i++) {
        Serial.print((int)command[i]);
        Serial.print(" ");
      }
      Serial.println();

      // Command handling
      if (command == "FWD") {
        Serial.println("ACTION: moveForward");
        moveForward();
      }
      else if (command == "BACK") {
        Serial.println("ACTION: moveBackward");
        moveBackward();
      }
      else if (command == "LEFT") {
        Serial.println("ACTION: turnLeft");
        turnLeft();
      }
      else if (command == "RIGHT") {
        Serial.println("ACTION: turnRight");
        turnRight();
      }
      else if (command == "STOP") {
        Serial.println("ACTION: stopMotors");
        stopMotors();
      }
      else if (command.startsWith("SERVO")) {
        int pan = command.substring(6, command.indexOf(' ', 6)).toInt();
        int tilt = command.substring(command.indexOf(' ', 6) + 1).toInt();
        Serial.print("ACTION: setServo pan=");
        Serial.print(pan);
        Serial.print(" tilt=");
        Serial.println(tilt);
        servoPan.write(pan);
        servoTilt.write(tilt);
      }
      else if (command == "PING") {
        float dist = readDistance();
        Serial.print("DIST ");
        Serial.println(dist);
      }
      else if (command.startsWith("CONFIG")) {
        parseConfig(command);
      }
      else {
        Serial.print("UNKNOWN COMMAND: ");
        Serial.println(command);
      }

      // Reset command buffer for next line
      command = "";
    }
  }
}

// Motor functions
void moveForward() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void moveBackward() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void turnRight() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

// Ultrasonic
float readDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

// Config handler
void parseConfig(String cmd) {
  Serial.println("ACTION: parseConfig");

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
