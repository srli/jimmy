#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import time
import serial
import string

def filter(line):
	numbed = []
	line = line.translate(string.maketrans("\n\r", "  "))
	line = line.strip()
	line = line.split(',')

	for num in line:
		if num == '':
			numbed.append(0)
		else:
#			num = float(num)
			numbed.append(num)

	return numbed
	
def publisher():
    rospy.init_node('axis_tilt', anonymous=True)

    pub = rospy.Publisher("axis_tilt", String)
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        lines = []
        for i in range(0,2):
            lines.append(ser.readline())
            #print lines
        result = []
#        for line in lines:
        result.append(filter(lines[1]))
        result = str(result)
    #    print result
        pub.publish(result)
        r.sleep()
	 
    
    
    
 
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0',9600)

#	ser = serial.Serial('/dev/ttyusb0', 9600)
    publisher()
#	while True:
#		lines = []
#		for i in range(0,2):
#			lines.append(ser.readline())
#		#print lines
#		result = []
#		#for line in lines:
#		result.append(filter(lines[1]))
#		print result
#	



