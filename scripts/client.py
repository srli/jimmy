#!/usr/bin/env python 

""" 
A simple echo client 
""" 
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import socket 



def remote_speech():
    pub = rospy.Publisher("remote_speech", String)
    rospy.init_node('client_hear', anonymous=True)
    r = rospy.Rate(10) # 10hz
    host = "10.7.24.104" #configured for jimmy stealth for now
    port = 50000 
    size = 1024
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    s.connect((host,port)) 
    while not rospy.is_shutdown():
        s.send('test string to ensure client is waiting') 
        data = s.recv(size) 
        print 'Received:', data
        if data == "end":
            s.close() 
        pub.publish(str(data))
        r.sleep()
#    print "Hello! My name is Jimmy. How are you?"
#    while not rospy.is_shutdown():
#        sentance = str(raw_input("What would you like to say?    "))
#        pub.publish(sentance)
#        r.sleep()

def remote_gesture():
    pub = rospy.Publisher("jimmy_send_gesture", String)
    rospy.init_node('client_hear', anonymous=True)
    r = rospy.Rate(10) # 10hz
    host = "10.7.24.104" #configured for jimmy stealth for now
    port = 50000 
    size = 1024
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    s.connect((host,port)) 
    while not rospy.is_shutdown():
        s.send('test string to ensure client is waiting') 
        data = s.recv(size) 
        print 'Received:', data
        if data == "end":
            s.close() 
        pub.publish(str(data))
        r.sleep()
#    print "Hello! My name is Jimmy. How are you?"
#    while not rospy.is_shutdown():
#        sentance = str(raw_input("What would you like to say?    "))
#        pub.publish(sentance)
#        r.sleep()


            
if __name__ == '__main__':
    control_type = raw_input("Control type?  ")
    if control_type == "speech":
        remote_speech()
    if control_type == "gesture":
        remote_gesture()