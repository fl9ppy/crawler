import cv2

class VideoCamera:
    def __init__(self, config):
        self.video = cv2.VideoCapture(0)
        self.face_cascade = cv2.CascadeClassifier(
            cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
        )
        self.width = config.get("camera", {}).get("width", 640)
        self.height = config.get("camera", {}).get("height", 480)
        self.video.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.video.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)

    def __del__(self):
        if self.video.isOpened():
            self.video.release()

    def get_frame(self):
        success, image = self.video.read()
        if not success:
            return None

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        faces = self.face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

        for (x, y, w, h) in faces:
            cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)

        ret, jpeg = cv2.imencode('.jpg', image)
        return jpeg.tobytes()

def gen(camera):
    while True:
        frame = camera.get_frame()
        if frame is None:
            continue
        yield (
            b'--frame\r\n'
            b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n'
        )

