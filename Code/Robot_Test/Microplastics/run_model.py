import cv2
from ultralytics import YOLO
import math

# 1. Load your trained model
# Make sure 'best.pt' is in the same folder as this script
model = YOLO("best.pt") 

# 2. Open the Video Stream
# Use '0' for Laptop Webcam
# Use '1' if you have an external USB camera plugged in
cap = cv2.VideoCapture(0) 

# Set resolution (optional, makes it run smoother)
cap.set(3, 640)
cap.set(4, 480)

print("Starting Microplastic Detector... Press 'q' to exit.")

while True:
    success, img = cap.read()
    if not success:
        print("Failed to read from camera!")
        break

    # 3. Run Inference (The AI Magic)
    # conf=0.5 means "Only show me detection if you are 50% sure"
    results = model(img, stream=True, conf=0.5)

    # 4. Draw the boxes
    for r in results:
        boxes = r.boxes
        for box in boxes:
            # Bounding Box
            x1, y1, x2, y2 = box.xyxy[0]
            x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
            
            # Confidence & Class Name
            conf = math.ceil((box.conf[0] * 100)) / 100
            cls = int(box.cls[0])
            class_name = model.names[cls]

            # Draw on the image
            cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(img, f'{class_name} {conf}', (x1, y1 - 10), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # 5. Show the video
    cv2.imshow('Hydro-Sentinel Laptop Test', img)

    # Press 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()