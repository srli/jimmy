#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
from jimmy.msg import *

def callback(data):
    global x_pos, y_pos
    print data
#    note output values are in order of x, y, z
#  
    raw_data = str(data)
    data1 = raw_data[6:]
    positions = data1.split(',')
#    y_pos = float(raw_data[15:19])
#    z_pos = float(raw_data[21:25])
    x_pos = int(positions[0])
    y_pos = int(positions[1])
    print [x_pos, y_pos]

    
def IMU_listen():
    global x_pos, y_pos
    global change_rate
    global ideal_position_x, ideal_position_y
    rospy.init_node('face_listen', anonymous=True)
    r = rospy.Rate(150)
    pub = rospy.Publisher("jimmy_move_servo", jimmy_servo)
    rospy.Subscriber("face_location", String, callback)
    ideal_position_x = 0
    ideal_position_y = 0
#    callback()
    while not rospy.is_shutdown():
        msg = jimmy_servo()
        msg.servo_names.append("HeadTilt2")
        ideal_position_x -= (x_pos - ideal_position_x)*change_rate
#        ideal_position_y -= (y_pos - ideal_position_y)*change_rate
        msg.positions.append(-ideal_position_x) # ideal_position_y)
        pub.publish(msg)
#        print "in here"
        r.sleep()
#    rospy.spin()
        
if __name__ == '__main__':
    try:
        x_pos = 150
        y_pos = 108
        ideal_position_x = 150
        ideal_position_y = 108
        change_rate = 0.00002 #you can change this to make arm move faster or slower        
        IMU_listen()

    except rospy.ROSInterruptException: pass
