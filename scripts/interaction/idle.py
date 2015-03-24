#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
from random import randint
import time
from std_msgs.msg import String
from jimmy.msg import *
from espeak import espeak

last_message_received_time = time.time()

def say(something):
    espeak.synth(something)

def callback(data):
    global last_message_received_time
    print "Registered idle interrupt message"
    last_message_received_time = time.time()

def idle_gesture(last_message_received_time, pub, r):
    if time.time() - last_message_received_time > 15:
        msg = jimmy_gesture()
        msg.cmd = randint(2,8)
        if msg.cmd == 8:
            time.sleep(8)
        lonely_speech = randint(1,20)
        if lonely_speech == 18:
            say("Hi, let's talk!")
        elif lonely_speech == 13:
            say("Is anyone there")
        elif lonely_speech == 8:
            say("I'm bored, talk to me")
        elif lonely_speech == 4:
            say("Hey you, you're awesome")
        print "published gesture", msg.cmd
        pub.publish(msg)
        time.sleep(7)
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


    pub = rospy.Publisher("jimmy_idle", jimmy_gesture)
    r = rospy.Rate(10)

    while not rospy.is_shutdown():
        idle_gesture(last_message_received_time, pub, r)

#    while True:
 #
#        if time.time() - last_message_received_time > 10:
#            # send a gesture here
#            msg = jimmy_gesture()
#            msg.cmd = randint(3,8)
#            print "published gesture", msg.cmd
#            pub.publish(msg)
#            print "Gesture published"
#            last_message_received_time = time.time()
#        r.sleep()

if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,150)
        espeak.set_parameter(espeak.Parameter.Pitch,99)
#        espeak.set_parameter(espeak.Parameter.Wordgap,)
        espeak.set_voice("en")
        listener()
    except rospy.ROSInterruptException: pass
