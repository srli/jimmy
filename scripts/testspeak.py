#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
import random
from std_msgs.msg import String
from espeak import espeak
from jimmy.msg import *
#include <jimmy/jimmy_gesture.h>

def say(something):
    espeak.synth(something)

def callback(data):
#    while not rospy.is_shutdown():
    raw = str(data)
    sentance = raw.split(' ',1)[1]
    print sentance
#    say(sentance)
    if 'hello' in sentance:      
        movegesture(random.randint(1,13))
        say("Hi! How are you?")
    if 'thank' in sentance:
        movegesture(random.randint(1,6))
        say("You're welcome.")        
    if 'work' in sentance:
        movegesture(random.randint(1,6))
        say("Yes, I'm ready to work. What should we do?")
    if 'move' in sentance:
        say("I can do that.")
        move("RightElbow", 0.5)
        print "Moving!"
    if 'back' in sentance:
        say("I can do that.")
        move("RightElbow", 0)
#    else:
#        say("What? I don't understand")

#        r.sleep()
#    say(sentance)

def move(servo, position):
    pub = rospy.Publisher("jimmy_move_servo", jimmy_servo)
    r = rospy.Rate(10) # 10hz
    msg = jimmy_servo()
    msg.servo_names.append(servo)
    msg.positions.append(position)
    pub.publish(msg)
#    rospy.spin()
    print "Message published!"
   
def movegesture(gesture):
    pub = rospy.Publisher("jimmy_send_gesture", jimmy_gesture)
    r = rospy.Rate(10)
    msg = jimmy_gesture()
    msg.cmd = gesture
    pub.publish(msg)
    print "Gesture published"
    
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("conversation", String, callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,150)
        espeak.set_parameter(espeak.Parameter.Pitch,99)
#        espeak.set_parameter(espeak.Parameter.Wordgap,)
        espeak.set_voice("en-sc")
        print "Ready to speak!"
        listener()
    except rospy.ROSInterruptException: pass
