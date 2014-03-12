/*Basically only listens to input from the ROS servo node
and writes to servos. */

#include "ControlUtils.h"
#include "Utils.h"

#include <ros/ros.h>
#include <ros/package.h>
#include <boost/thread.hpp>
#include <jimmy/jimmy_servo.h>
#include <jimmy/jimmy_command.h>
 
//#define SIMULATION

static const int8_t default_gain[TOTAL_JOINTS] = 
{
  32, 32, 60, 32, 60, 32,  
  32, 32, 60, 32, 60, 32,  
  32, 32, 32, 32,
  32, 32, 32, 32,
  32, 32, 32
};

static const int8_t walk_gain[TOTAL_JOINTS] = {
  40, 60, 120, 60, 60, 120,
  40, 60, 120, 60, 60, 120,
  32, 32, 32, 32,
  32, 32, 32, 32,
  32, 32, 32
};
#ifndef SIMULATION
ControlUtils utils;  // talks to all the servos
#endif
///////////////////////////////////////////////////
// ros stuff
boost::mutex r_Lock;
static double r_vFwd = 0;
static double r_vLeft = 0;
static double r_dTheta = 0;
static int r_mode = 0;
static double r_neckEAd[3] = {0};


ros::Publisher pub_feedback;

// wipes all stored commands
void cleanCommand()
{
  boost::mutex::scoped_lock lock(r_Lock);
  r_vFwd = 0;
  r_vLeft = 0;
  r_dTheta = 0;
  r_mode = 0;
  r_neckEAd[0] = 0; 
  r_neckEAd[1] = 0; 
  r_neckEAd[2] = 0; 
}

// gets called by ros, copies ros land commands to cache
void jimmyServoCallback(const jimmy::jimmy_servo &msg)
{
  printf("In servo callback\n");

  assert(utils.getJoints());
//  boost::mutex::scoped_lock lock(r_Lock);

  double jointPositions[23];

  utils.getJoints(jointPositions);
  
  for (int i = 0; i < msg.positions.size(); i++) {
  	jointPositions[msg.servo_numbers[i]] = msg.positions[i];
	  std::cout << "setting joint  " << msg.servo_numbers[i] << "  at position  " << msg.positions[i] << "  in radians" << std::endl;
  }

  utils.setJoints(jointPositions);
}
///////////////////////////////////////////////////


int main( int argc, char **argv ) 
{
  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "jimmy_servos", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();
  printf("Ready to listen for commands!\n");

  //Subscribes to jimmy_move_servo, which tells us which servos to move
  ros::Subscriber subcommand = rosnode.subscribe("jimmy_move_servo", 10, jimmyServoCallback);
	

    ///////////////////////////////////////////////
    // proc ros msg
   for (;;) {
     //printf("Spinning!\n");
     ros::spinOnce();
     sleep(1);
   }
	return 0;
}



