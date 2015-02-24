#!/usr/bin/python

#----------------------------------------------------------------------------
# Face Detection Test (OpenCV)
#
# thanks to:
# http://japskua.wordpress.com/2010/08/04/detecting-eyes-with-python-opencv2
#----------------------------------------------------------------------------

import cv2
import time
import Image
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String

HAAR_CASCADE_PATH = "/opt/ros/indigo/share/OpenCV/"

class Face:
    def __init__(self,x,y,w,h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.midx = x + w/2
        self.midy = y + h/2

#Input: list of faces
#Return: index of widest face
def largestFaceIndex(faces):
    largestIndex = 0;
    largest = faces[0].x
    for index, face in enumerate(faces):
        if face.w > largest:
            largest = face.w
            largestIndex = index
    return largestIndex

def DetectFace(image, faceCascade):
    min_size = (20,20)
    image_scale = 2
    haar_scale = 1.1
    min_neighbors = 3
    haar_flags = 0

    # Allocate the temporary images
    grayscale = cv2.CreateImage((image.width, image.height), 8, 1)
    smallImage = cv2.CreateImage(
            (
                cv2.Round(image.width / image_scale),
                cv2.Round(image.height / image_scale)
            ), 8 ,1)

    # Convert color input image to grayscale
    cv2.CvtColor(image, grayscale, cv2.CV_BGR2GRAY)

    # Scale input image for faster processing
    cv2.Resize(grayscale, smallImage, cv2.CV_INTER_LINEAR)

    # Equalize the histogram
    cv2.EqualizeHist(smallImage, smallImage)

    # Detect the faces
    faces = cv2.HaarDetectObjects(
            smallImage, faceCascade, cv2.CreateMemStorage(0),
            haar_scale, min_neighbors, haar_flags, min_size
        )

    # If faces are found
    faceObjects = []

    if faces:

        for ((x, y, w, h), n) in faces:
            faceObjects.append(Face(x,y,w,h))
            # the input to cv2.HaarDetectObjects was resized, so scale the
            # bounding box of each face and convert it to two CvPoints
            pt1 = (int(x * image_scale), int(y * image_scale))
            pt2 = (int((x + w) * image_scale), int((y + h) * image_scale))
            cv2.Rectangle(image, pt1, pt2, cv2.RGB(0, 100, 255), 5, 8, 0)

    return [image,faceObjects]

def publisher():
    rospy.init_node("face_location",anonymous = True)
    # capture = cv2.CaptureFromCAM(1)
    capture = cv2.VideoCapture(0)
    #capture = cv2.CaptureFromFile("test.avi")

    #faceCascade = cv2.Load("haarcascades/haarcascade_frontalface_default.xml")
    #faceCascade = cv2.Load("haarcascades/haarcascade_frontalface_alt2.xml")
    faceCascade = cv2.Load(HAAR_CASCADE_PATH + "haarcascades/haarcascade_frontalface_alt.xml")
    
    pub = rospy.Publisher('face_location', String)
    while not rospy.is_shutdown():
        while (cv2.WaitKey(15)==-1):
            img = cv2.QueryFrame(capture)
            if not img:
                print "No camera"
            [image,faces] = DetectFace(img, faceCascade)
            faces.sort(key=lambda face: face.y)
            if len(faces) != 0:
                #Replacing top face with largest face
                # face = faces[0]
                # print "top face at:" + str(face.midx) + ", "+ str(face.midy)
                face = faces[largestIndex(faces)]
                print "largest face at:" + str(face.midx) + ", "+ str(face.midy)
                pub.publish(str(face.midx)+","+str(face.midy))
            cv2.ShowImage("face detection test", image)

if __name__ == "__main__":
    publisher()


    #From Doc's Right Eye
    #Doc Appears to be looking at you when
    #top face at:150, 108