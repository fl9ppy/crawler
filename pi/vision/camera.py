import cv2

class VideoCamera:
    def __init__(self, config):
        self.stream = cv2.VideoCapture(0)
        self.stream.set(cv2.CAP_PROP_FRAME_WIDTH, config["camera"]["resolution"][0])
        self.stream.set(cv2.CAP_PROP_FRAME_HEIGHT, config["camera"]["resolution"][1])
        self.stream.set(cv2.CAP_PROP_FPS, config["camera"]["framerate"])

        cascade_path = cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
        self.face_cascade = cv2.CascadeClassifier(cascade_path)

        self.frame_count = 0
        self.face_boxes = []

    def get_frame(self):
        success, frame = self.stream.read()
        if not success:
            return None

        self.frame_count += 1

        if self.frame_count % 15 == 0:
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            self.face_boxes = self.face_cascade.detectMultiScale(
                gray,
                scaleFactor=1.1,
                minNeighbors=5,
                minSize=(30, 30)
            )

        for (x, y, w, h) in self.face_boxes:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        _, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()

    def release(self):
        self.stream.release()

def gen(camera):
    while True:
        frame = camera.get_frame()
        if frame:
            yield (b"--frame\r\n"
                   b"Content-Type: image/jpeg\r\n\r\n" + frame + b"\r\n\r\n")

