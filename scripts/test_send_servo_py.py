#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
#from std_msgs.msg import String
from jimmy.msg import *

def send_servo():
    pub = rospy.Publisher('jimmy_move_servo', jimmy_servo)
    rospy.init_node('test_send_servo_cmd_py')
    r = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        raw_input_command = raw_input("Please input desired servo and position: ")
        input_command = raw_input_command.split()
        print input_command
        if input_command[0] == "end":
            break
        i = 0
        msg = jimmy_servo()
        while i < len(input_command):
            msg.positions.append(int(input_command[i+1]))
            if input_command[i].isalpha():
                print "adding this to servo names"
                msg.servo_names.append(input_command[i])
            elif input_command[i].isdigit():
                print "adding this to servo numbers"
                msg.servo_numbers.append(int(input_command[i]))
            i += 2 
#        rospy.loginfo(msg)
        pub.publish(msg)
        r.sleep()
        
if __name__ == '__main__':
    try:
#        pub = rospy.Publisher('jimmy_move_servo',jimmy_servo)
#        rospy.init_node('test_send_servo_cmd_py')
#        r = rospy.Rate(1) # 10hz
#        while not rospy.is_shutdown():
#            msg = jimmy_servo()
#            msg.servo_names.append("LeftElbow")
#            msg.positions.append(0.5)
#            pub.publish(msg)
#            r.sleep()
        send_servo()
    except rospy.ROSInterruptException: pass
