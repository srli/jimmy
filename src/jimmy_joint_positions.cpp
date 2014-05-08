#include "ControlUtils.h"
#include "Utils.h"
#include <ros/ros.h>
#include <ros/package.h>
#include "RobotState.h"
#include "std_msgs/String.h"


static double joints_actual[TOTAL_JOINTS];

int main()
{

  ros::init("joint_positions", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();
 	
  ros::Publisher pub = rosnode.advertise<std_msgs::String>("jimmy_joint_positions", 10);
  std_msgs::String msg;

  ControlUtils utils;
  std::stringstream ss;
  //TO-DO: Make values of msg a table, push back each joint name etc.
  while(true){
    utils.getJoints(joints_actual);
    printf("Joints succesfully retrieved!\n");
    for (int i = 0; i < TOTAL_JOINTS; i++) {
    	//ss << RobotState::jointNames[i].c_str() << joints_actual[i]) << count;
    	ss << joints_actual[i] << count;
    	msg.data = ss.str(); 
        printf("%10s %g\n", RobotState::jointNames[i].c_str(), joints_actual[i]);  
      }
    }

  pub.publish(msg);
  for (int i = 0; i < 10000; i++){
    ros::spinOnce();
    sleep(5);
}  
}
