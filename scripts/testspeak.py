#!/usr/bin/env python

from pattern.en import *    
import pygame
import rospy
from std_msgs.msg import String

def callback(data):
    sentance = str(data)
    if positive(sentance, threshold = 0.2):
        print ("That's nice!")
        pygame.mixer.music.load('media/laughing.wav')
        pygame.mixer.music.play()
        while pygame.mixer.music.get_busy(): 
            pygame.time.Clock().tick(1)
    else:
        print ("That's sad.")
        pygame.mixer.music.load('media/sad.wav')
        pygame.mixer.music.play()
        while pygame.mixer.music.get_busy(): 
            pygame.time.Clock().tick(1)

    
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("conversation", String, callback)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
        
if __name__ == '__main__':
    try:
        pygame.init()
        pygame.mixer.init() 
        listener()
    except rospy.ROSInterruptException: pass

