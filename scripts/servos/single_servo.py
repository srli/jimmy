#!/usr/bin/env python
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import Int16

def main():
    rospy.init_node('send_single_servo')
    print "This is the Single Servo test for the Arbotix board"
    pub = rospy.Publisher('single_servo', Int16)
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        servo_num = raw_input("What servo to move?: ")
        try:
            servo_num = int(servo_num)
        except:
            print "Didn't input number, defaulting to servo 1"
            servo_num = 1
        pub.publish(servo_num)
        r.sleep()
    rospy.spin()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException: pass
