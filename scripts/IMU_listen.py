#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
from jimmy.msg import *
import time

last_IMU_callback = time.time()



def callback(data):
    pub = rospy.Publisher("jimmy_move_servo", jimmy_servo)

#    note output values are in order of x, y, z
    raw_data = str(data)
    x_pos = float(raw_data[9:13])
    y_pos = float(raw_data[15:19])
    z_pos = float(raw_data[21:25])
    
    print [x_pos, y_pos, z_pos]
    r = rospy.Rate(10)
    msg = jimmy_servo()

    msg.servo_names = "LeftElbow"
    if x_pos > 1.5:
        msg.positions = 0.01
        #move servo up
    elif x_pos < 1:
        msg.positions = -0.01
    else:
        msg.positions = 0
        
    pub.publish(msg)
    r.sleep()
        #move servo down
#    print str(data)[9:13]
#    global last_IMU_callback
#    if time.time() - last_IMU_callback > 1:
#        print data
#        print "Getting IMU values now"
#        last_IMU_callback = time.time()      


def IMU_listen():
    rospy.init_node('IMU_listen', anonymous=True)
#    rospy.Subscriber("systemsounds", String, system)
    rospy.Subscriber("axis_tilt", String, callback)
    rospy.spin()
        
if __name__ == '__main__':
    try:
        IMU_listen()
    except rospy.ROSInterruptException: pass
