#!/usr/bin/env python 

""" 
A simple echo client 
""" 
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import socket 



def talker():
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

def publisher():
    rospy.init_node('client_hear', anonymous=True)
    pub = rospy.Publisher("remote_speech", String)
    data = "hello"
    pub.publish(data)
    print "data published!"
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
        pub.publish(data)

            
if __name__ == '__main__':
    talker()