#!/usr/bin/python

import cv2
import time
import Image
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import numpy as np

#Each face that is detected by the webcam will be made as a class Face
#which has position and size
class Face:
    def __init__(self,x,y,w,h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.midx = x + w/2
        self.midy = y + h/2

def largestFaceIndex(faces):
    """Takes in a list of faces and returns the largest face index"""
    
    largestIndex = 0;
    largest = 0
    for index, face in enumerate(faces):
        if face.w > largest:
            largest = face.w
            largestIndex = index
    return largestIndex

def publisher():
    """Scans for faces, publishes the position of the largest face"""

    #Initialize rospy
    rospy.init_node("face_location",anonymous = True)
    
    #Get video stream from the cam and set the resolution
    cap = cv2.VideoCapture(0)
    cap.set(3, 480)
    cap.set(4, 360)

    #Define the location of the haar cascades
    face_cascade = cv2.CascadeClassifier('/home/skelly1/opencv-2.4.10/data/haarcascades/haarcascade_frontalface_alt.xml')

    #Initialize publish
    pub = rospy.Publisher('face_location', String)

    while not rospy.is_shutdown():
        
        #Define the current cam image as 'frame'
        ret, frame = cap.read()

        faces = face_cascade.detectMultiScale(frame, scaleFactor=1.2, minSize=(20,20))
        #For each face detected, create a Face class with its characteristics
        if len(faces) > 0:
            faceObjects = []
            for (x, y, w, h) in faces:
                faceObjects.append(Face(x,y,w,h))

            for (x,y,w,h) in faces:
                cv2.rectangle(frame,(x,y),(x+w,y+h),(0,0,255))

            if len(faceObjects) != 0:
                #Determine the largest face of the array 'faceObjects'
                face = faceObjects[largestFaceIndex(faceObjects)]

                #Print and publish the x,y position of the largest face
                print "largest face at: "+str(face.midx)+", "+str(face.midy)
                pub.publish(str(face.midx)+","+str(face.midy))

        #Display the final image
        cv2.imshow('frame',frame)

        #If ESC is pressed, quit the program
        if cv2.waitKey(1) & 0xFF == 27:
            break

    #Release the capture and close the windows
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    publisher()