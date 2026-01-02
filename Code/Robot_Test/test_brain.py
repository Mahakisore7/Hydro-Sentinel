from ultralytics import YOLO
import cv2

# 1. Load YOUR custom trained brain
# Make sure the file name matches exactly what you downloaded
model = YOLO("my_river_model.pt")

# 2. Open the Webcam (0 is default, try 1 if you have an external cam)
cap = cv2.VideoCapture(0)

# Check if camera opened successfully
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

print("✅ Robot Vision Active. Press 'Q' to quit.")

while True:
    ret, frame = cap.read()
    if not ret: break

    # 3. The AI scans the frame
    # conf=0.4 means "Only alert if you are 40% sure"
    results = model(frame, conf=0.4)

    # 4. Draw the bounding boxes (Red Squares)
    annotated_frame = results[0].plot()

    # 5. Show the video feed
    cv2.imshow("River Robot AI - Test Mode", annotated_frame)

    # Press 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()