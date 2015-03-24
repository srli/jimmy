import cv2
import numpy as np

cap = cv2.VideoCapture(0)

while(1):

    # Take each frame
    _, frame = cap.read()

    # Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
    # lower_blue = np.array([200,50,0])
    # upper_blue = np.array([255,150,50])

    lower_blue = np.array([50, 175, 125])
    upper_blue = np.array([100, 255, 200])


    # Threshold the HSV image to gently blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    width, height = mask.shape
    print width
    print height

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)

    cv2.imshow('frame',frame)
    cv2.imshow('mask',mask)
    cv2.imshow('res',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()