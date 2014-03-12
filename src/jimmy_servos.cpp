/*
 * Main controller for Jimmy. 
 * 
 * The main loop runs at 100hz, it can be changed by plan.cf/TIME_STEP. 
 * The controller is open loop, as it plans a motion, generates joint 
 * angles and sends them to the servos. It does not react to perturbations.
 *
 * The main loop does the following on every tick:
 *  1. Processes external command, such as switching modes
 *  2. Generates IK_d based on walking or gesturing behavior, 
 *  3. Uses IK to generate the desired joint angles, q_d. 
 *  4. Sends q_d to servos with sync write.
 *
 * On every tick, we have to send q_d to all the joints. q_d are send to all
 * the servos with sync write. Since there isn't a sync read, and polling 
 * every joint sequentially is too slow, we read 2 joints each tick. Reading
 * current joint angle is only for logging and debugging behavior. 
 */

#include "Plan.h"
#include "Logger.h"
#include "IK.h"
#include "ControlUtils.h"
#include "Utils.h"

#include <ros/ros.h>
#include <ros/package.h>
#include <boost/thread.hpp>
#include <time.h>
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
ControlUtils utils;         // talks to all the servos
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
    printf("setting joints at\n");
	  std::cout << msg.positions[i] << " : " << msg.servo_numbers[i] << std::endl;
  }

  utils.setJoints(jointPositions);
}
///////////////////////////////////////////////////

Plan plan;                  // walking pattern generator
Logger logger;              // data logging
IKcmd IK_d;                 // desired quantities for IK
IKcon IK;                   // full body IK solver

void spin_wait(double dt)
{
  double t0 = get_time();
  int spin = 1;
  int ctr = 0;
  while(spin) {
    if (get_time() - t0 < dt) {
      ctr++;
    }
    else
      spin = 0;
  }
  return;
}

int main( int argc, char **argv ) 
{
  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "jimmy_servos", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();
  

  ros::Time last_ros_time_;
  bool wait = true;
  while (wait) {
    last_ros_time_ = ros::Time::now();
    if (last_ros_time_.toSec() > 0) {
      wait = false;
    }
  }

  ros::Subscriber subcommand = rosnode.subscribe("jimmy_move_servo", 10, jimmyServoCallback);
	

    ///////////////////////////////////////////////
    // proc ros msg
   for (;;) {
     //printf("Spinning!\n");
     ros::spinOnce();
     sleep(1);
   }

    ///////////////////////////////////////////////
    // wait
   ///////////////////////////////////////////////


	return 0;
}



