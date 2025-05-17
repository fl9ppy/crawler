import RPi.GPIO as GPIO
import time

# Define motor control pins
IN1 = 17  # GPIO17 (pin 11)
IN2 = 27  # GPIO27 (pin 13)
ENA = 22  # GPIO22 (pin 15) - PWM for speed

IN3 = 23  # OPTIONAL second motor (e.g. GPIO23)
IN4 = 24  # OPTIONAL second motor (e.g. GPIO24)
ENB = 25  # OPTIONAL PWM for second motor (GPIO25)

# Setup GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(IN1, GPIO.OUT)
GPIO.setup(IN2, GPIO.OUT)
GPIO.setup(ENA, GPIO.OUT)

# If you use second motor:
GPIO.setup(IN3, GPIO.OUT)
GPIO.setup(IN4, GPIO.OUT)
GPIO.setup(ENB, GPIO.OUT)

# Create PWM instances
pwmA = GPIO.PWM(ENA, 1000)  # 1kHz
pwmB = GPIO.PWM(ENB, 1000)

pwmA.start(0)
pwmB.start(0)

def set_speed(speed=100):
    pwmA.ChangeDutyCycle(speed)
    pwmB.ChangeDutyCycle(speed)

def forward():
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    set_speed(100)

def backward():
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.HIGH)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.HIGH)
    set_speed(100)

def turn_left():
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.HIGH)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    set_speed(100)

def turn_right():
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.HIGH)
    set_speed(100)

def stop():
    pwmA.ChangeDutyCycle(0)
    pwmB.ChangeDutyCycle(0)
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)

def cleanup():
    stop()
    pwmA.stop()
    pwmB.stop()
    GPIO.cleanup()

