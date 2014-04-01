#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from espeak import espeak

def say(something):
    espeak.synth(something)

def callback(data):
    raw = str(data)
    sentance = raw.split(' ',1)[1]
    print sentance
    say(sentance)
   
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("conversation", String, callback)
    rospy.spin()
        
if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,150)
        espeak.set_parameter(espeak.Parameter.Pitch,99)
        listener()
    except rospy.ROSInterruptException: pass
