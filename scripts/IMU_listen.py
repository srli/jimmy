#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
from jimmy.msg import *

def callback(data):
    global x_pos

#    note output values are in order of x, y, z
  
    raw_data = str(data)
    x_pos = float(raw_data[9:13])
    y_pos = float(raw_data[15:19])
    z_pos = float(raw_data[21:25])
    
    print [x_pos, y_pos, z_pos]

    
def IMU_listen():
    global x_pos
    global change_rate
    global ideal_position
    rospy.init_node('IMU_listen', anonymous=True)
    r = rospy.Rate(100)
    pub = rospy.Publisher("jimmy_move_servo", jimmy_servo)
    rospy.Subscriber("axis_tilt", String, callback)
    while not rospy.is_shutdown():
        msg = jimmy_servo()
        msg.servo_names.append("LeftElbow")
        ideal_position -= (x_pos - 1.5)*change_rate
        msg.positions.append(ideal_position)
        pub.publish(msg)
#        print "in here"
        r.sleep()
#    rospy.spin()
        
if __name__ == '__main__':
    try:
        ideal_position = -.9
        x_pos = 1.5
        change_rate = 0.02 #you can change this to make arm move faster or slower        
        IMU_listen()

    except rospy.ROSInterruptException: pass
