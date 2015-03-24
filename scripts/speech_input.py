#!/usr/bin/env python

import time
import roslib; roslib.load_manifest('jimmy')
import rospy
from std_msgs.msg import String
import numpy as np

def publisher():

	rospy.init_node("textConversation",anonymous = True)

	pub = rospy.Publisher("textConversation", String, queue_size = 10)

	while not rospy.is_shutdown():

		# inputText = input('Say something to Jimmy: ')
		pub.publish(raw_input('Say something to Jimmy: '))

if(__name__ == '__main__'):
	publisher()