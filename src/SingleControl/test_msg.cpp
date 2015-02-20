/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Single Control Sample                                *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
// General headers
#include <stdio.h>
//ROS headers
#include <ros/ros.h>
#include <ros/package.h> //we use this to get file paths

#include <stdlib.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <jimmy/pointerpos.h>
#include <jimmy/jimmy_gesture.h>

#include <sstream>

/*int xpos;
int ypos;
int zpos;
*/
bool wave;
bool sess_start;
bool sess_end;

// this sample can run either as a regular sample, or as a client for multi-process (remote mode)
int main(int argc, char** argv)
{
	//We initialize our ROS nodes and publishers here
	ros::init(argc, argv, "pointcontrol", ros::init_options::NoSigintHandler);
  	ros::NodeHandle rosnode = ros::NodeHandle();

/*  	ros::Publisher pub = rosnode.advertise<jimmy::pointerpos>("point_location", 10); //publisher for hand XYZ positions
  	jimmy::pointerpos msg;
*/
  	ros::Publisher pub_gestures = rosnode.advertise<jimmy::jimmy_gesture>("detected_gestures", 10); //publisher for gesture booleans
  	jimmy::jimmy_gesture msg_gestures;


	//fill out the custom message fields with the placeholders we've previously defined
/*	msg.positionx = xpos;
	msg.positiony = ypos;
	msg.positionz = zpos;
	pub.publish(msg);
*/
	for (;;)
	{
		msg_gestures.wave = wave;
		msg_gestures.hello = sess_start;
		msg_gestures.goodbye = sess_end;
		pub_gestures.publish(msg_gestures);

		//set gesture booleans back to false for re-initialization again
		wave = true;
		sess_start = false;
		sess_end = false;

		ros::spinOnce();
		sleep(5);
	}
	return 0;
}
