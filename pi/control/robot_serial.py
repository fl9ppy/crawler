import serial
import time

class RobotSerial:
    def __init__(self, port="/dev/ttyUSB0", baudrate=9600):
        try:
            self.ser = serial.Serial(port, baudrate, timeout=1)
            time.sleep(2)  # Let Arduino boot
            print("[RobotSerial] Connected to Arduino.")
        except serial.SerialException as e:
            print(f"[ERROR] Could not open serial port: {e}")
            self.ser = None

    def send(self, command):
        if self.ser:
            self.ser.write((command + "\n").encode())

    def read_line(self):
        if self.ser and self.ser.in_waiting:
            return self.ser.readline().decode().strip()
        return None

    def move(self, direction):
        self.send(direction)

    def set_servo(self, pan, tilt):
        self.send(f"SERVO {pan} {tilt}")

    def read_distance(self):
        self.send("PING")
        start = time.time()
        while time.time() - start < 1.0:
            line = self.read_line()
            if line and line.startswith("DIST"):
                try:
                    return float(line.split()[1])
                except ValueError:
                    return -1
        return -1

