import cv2
import numpy as np

cap = cv2.VideoCapture(0)

while(1):

    # Take each frame
    _, frame = cap.read()
    blurframe = cv2.blur(frame, (50, 50))

    # Convert BGR to HSV
    hsv = cv2.cvtColor(blurframe, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
    lower_blue = np.array([40, 165, 115])
    upper_blue = np.array([100, 255, 200])

    # Threshold the HSV image to get only blue colors
    cv2.medianBlur(hsv, 3)
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    ret,thresh = cv2.threshold(frame,127,255,0)
    contours,hierarchy = cv2.findContours(mask, 1, 2)

    if len(contours) > 0:
        cnt = contours[0]
        M = cv2.moments(cnt)
        x,y,w,h = cv2.boundingRect(cnt)
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0, 0, 255), 3)
        # print 'Stuff:', x, y
        # cv2.circle(frame, (x,y), 10, (0, 255, 0))

    # Bitwise-AND mask and original image
    # res = cv2.bitwise_and(frame,frame, mask= mask)

    cv2.imshow('frame',frame)
    # cv2.imshow('mask',mask)
    # cv2.imshow('res',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()