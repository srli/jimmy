#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
import random
from time import sleep
from std_msgs.msg import String
from espeak import espeak
from jimmy.msg import *

def callback(data):
    global user_input
    print data
 
    user_input = str(data)
    print user_input

def listener():
    rospy.init_node('speech_listen', anonymous=True)
    rospy.Subscriber("textConversation", String, callback)

    while not rospy.is_shutdown():
        user_input = str(data).lower()
        sentence = user_input.split(' ',1)[1]
        print sentence
        #    say(sentence)
        if 'hello' in sentence:      
            sleep(2)                                                                                                                                                                                                                                                                                                                                                                                                                
            say("Hello! How are you.")
            return
        if 'thank' in sentence:
            sleep(2)
            say("You're welcome.")    
            return
        if 'your name' in sentence:
            sleep(1)
            say("My name is Jimmy.")
            return
        if "haha" in sentence:
            sleep(2)
            say("haha, i am laughing")
            return
        else:
            sleep(2)
            say("Hahaha")
            sleep(1)
            say("It is funny that you think my master was intelligent enough to allow me to respond to that.")
            return

if __name__ == '__main__':
    listener()