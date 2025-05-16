import serial
import time

class RobotSerial:
    def __init__(self, config):
        port = config["serial"]["port"]
        baudrate = config["serial"]["baudrate"]
        self.ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)
        print("[RobotSerial] Connected")

        # Send dynamic pin config
        trig = config["pins"]["trig"]
        echo = config["pins"]["echo"]
        pan_pin = config["pins"]["pan"]
        tilt_pin = config["pins"]["tilt"]
        self.send(f"CONFIG TRIG={trig} ECHO={echo} PAN={pan_pin} TILT={tilt_pin}")
        time.sleep(0.1)

        # Send initial servo position
        pan = config["servos"].get("pan_start", 90)
        tilt = config["servos"].get("tilt_start", 90)
        self.set_servo(pan, tilt)

    def send(self, command):
        self.ser.write((command + "\n").encode())

    def read_line(self):
        if self.ser.in_waiting:
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

