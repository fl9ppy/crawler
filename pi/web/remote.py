import os
from flask import Flask, render_template, request, jsonify, Response

from control.robot_serial import RobotSerial
from vision.camera import VideoCamera, gen

# Flask setup
BASE_DIR = os.path.abspath(os.path.dirname(__file__))
TEMPLATE_DIR = os.path.join(BASE_DIR, "templates")
app = Flask(__name__, template_folder=TEMPLATE_DIR)

# Hardware interface
bot = RobotSerial()
camera = VideoCamera(config={"camera": {"resolution": (640, 480), "framerate": 30}})

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/move", methods=["POST"])
def move():
    direction = request.form.get("direction", "STOP").upper()
    bot.move(direction)
    return "OK"

@app.route("/camera", methods=["POST"])
def camera_control():
    pan = int(request.form.get("pan", 90))
    tilt = int(request.form.get("tilt", 90))
    bot.set_servo(pan, tilt)
    return "OK"

@app.route("/distance")
def distance():
    dist = bot.read_distance()
    return jsonify({"distance_cm": dist})

@app.route("/video_feed")
def video_feed():
    return Response(gen(camera),
                    mimetype="multipart/x-mixed-replace; boundary=frame")

def run():
    app.run(host="0.0.0.0", port=5000, debug=False)

