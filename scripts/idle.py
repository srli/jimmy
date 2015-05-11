#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
from random import randint
import time
from std_msgs.msg import String, Int16
from jimmy.msg import *
from espeak import espeak

last_message_received_time = time.time()

def say(something):
    espeak.synth(something)

def lonely_speech():
    lonely_speech = randint(1,20)
    if lonely_speech == 18:
        say("Hi, let's talk!")
    elif lonely_speech == 13:
        say("Is anyone there")
    elif lonely_speech == 8:
        say("I'm bored, talk to me")
    elif lonely_speech == 4:
        say("Hey you, you're awesome")

def callback(data):
    global last_message_received_time
    print "Registered idle interrupt message"
    last_message_received_time = time.time()

def idle_gesture(last_message_received_time, pub, r):
    if time.time() - last_message_received_time > 5:
        lonely_speech()
        msg = Int16()
        msg.data = randint(1,8)
        print "published gesture", msg.data
        pub.publish(msg)
        time.sleep(5)
        print "Gesture published"
        last_message_received_time = time.time()
        r.sleep()
    else:
        return


def listener():
    print "Starting to idle!"
    global last_message_received_time
    rospy.init_node('idle_node', anonymous=True)

    rospy.Subscriber("conversation", String, callback)
    rospy.Subscriber("jimmy_send_gesture", jimmy_gesture, callback)
    rospy.Subscriber("jimmy_send_servo", jimmy_servo, callback)
    rospy.Subscriber("axis_tilt", String, callback)


    pub = rospy.Publisher("idle", Int16)
    r = rospy.Rate(10)

    while not rospy.is_shutdown():
        idle_gesture(last_message_received_time, pub, r)

if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,150)
        espeak.set_parameter(espeak.Parameter.Pitch,99)
#        espeak.set_parameter(espeak.Parameter.Wordgap,)
        espeak.set_voice("en")
        listener()
    except rospy.ROSInterruptException: pass
