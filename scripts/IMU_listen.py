#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
from jimmy.msg import *

def callback(data):
    pub = rospy.Publisher("jimmy_move_servo", jimmy_servo)
    r = rospy.Rate(10)
    global ideal_position

#    note output values are in order of x, y, z
#    while not rospy.is_shutdown():
    
    raw_data = str(data)
    x_pos = float(raw_data[9:13])
    y_pos = float(raw_data[15:19])
    z_pos = float(raw_data[21:25])
    
    print [x_pos, y_pos, z_pos]

    
    msg = jimmy_servo()

    msg.servo_names.append("LeftElbow")
#        ideal_position = 0

    if x_pos > 1.5:
        ideal_position -= 0.03
        x_pos = 1.25
        print "ideal position is", ideal_position
        #move servo up
    elif x_pos < 0.75:
        ideal_position += 0.03
        x_pos = 1.25
        print "ideal position is", ideal_position
#            
#    else:
#        msg.positions.append(0)
    msg.positions.append(ideal_position)
    pub.publish(msg)
#    r.sleep()
    
def IMU_listen():
    rospy.init_node('IMU_listen', anonymous=True)
    rospy.Subscriber("axis_tilt", String, callback)
    rospy.spin()
        
if __name__ == '__main__':
    try:
        ideal_position = 0
        IMU_listen()
    except rospy.ROSInterruptException: pass
