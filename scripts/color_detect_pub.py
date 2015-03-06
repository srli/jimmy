#!/usr/bin/python

import cv2
import time
import Image
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import numpy as np

def publisher():

    rospy.init_node("color_location",anonymous = True)

    #Start cam stream
    cap = cv2.VideoCapture(1)

    #Initialize publish
    pub = rospy.Publisher('color_location', String, queue_size = 10)

    while not rospy.is_shutdown():

        #Set 'frame' as the current cam capture
        _, frame = cap.read()

        #Blur the frame
        blurframe = cv2.blur(frame, (50, 50))

        #Convert BGR to HSV
        # hsv = cv2.cvtColor(blurframe, cv2.COLOR_BGR2HSV)

        #Define color range in BGR
        lower_blue = np.array([50, 200, 50])
        upper_blue = np.array([150, 255, 150])

        # Threshold the HSV image to get only the selected colors
        # cv2.medianBlur(blurframe, 3)
        mask = cv2.inRange(blurframe, lower_blue, upper_blue)
        contour_mask = mask
        cv2.imshow('mask', mask)

        #Create a contour around the selected colors
        # ret,thresh = cv2.threshold(frame,127,255,0)
        _,contours,_ = cv2.findContours(contour_mask, 1, 2)

        #If there is a contour, create a rectangle around it and print location
        if len(contours) > 0:
            cnt = contours[0]
            M = cv2.moments(cnt)
            x,y,w,h = cv2.boundingRect(cnt)
            cv2.rectangle(frame, (x,y), (x+w, y+h), (0, 0, 255), 3)
            print x, y
            pub.publish(str(x)+","+str(y))

        #Display the final image
        cv2.imshow('frame', frame)
        cv2.imshow('blur', blurframe)

        #If ESC pressed, quit the program
        if cv2.waitKey(1) & 0xFF == 27:
            break

    #Release the capture and close the windows
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    publisher()