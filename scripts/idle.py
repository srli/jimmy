#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
from random import randint
from time import sleep
from std_msgs.msg import String
from jimmy.msg import *

def callback(data):
    global idle 
    idle = True
    print "IDLE motion interrupted"
    sleep(15)
    idle = False
    print "exit callback"
    #sleep(15) #wait 5 seconds when we hear input from somewhere else
    
def listener():
    rospy.init_node('idle_node', anonymous=True)

    rospy.Subscriber("conversation", String, callback)
    rospy.Subscriber("jimmy_send_gesture", jimmy_gesture, callback)
    rospy.Subscriber("jimmy_send_servo", jimmy_servo, callback)
    
    pub = rospy.Publisher("jimmy_idle", jimmy_gesture)
    r = rospy.Rate(10)
    while not idle:
        msg = jimmy_gesture()
        msg.cmd = randint(3,8)
        print "published gesture", msg.cmd
        pub.publish(msg)
        print "Gesture published"
        sleep(7)
    
    rospy.spin()

if __name__ == '__main__':
    try:
        idle = False
#        espeak.set_parameter(espeak.Parameter.Rate,150)
#        espeak.set_parameter(espeak.Parameter.Pitch,99)
##        espeak.set_parameter(espeak.Parameter.Wordgap,)
#        espeak.set_voice("en-sc")
#        print "Ready to speak!"
        listener()
    except rospy.ROSInterruptException: pass
