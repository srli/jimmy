//This script subscribes to single_servo node and will perform actions regarding that servo according to inputs

#include <Arduino.h>
#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"
#include "jimmy_arms.h"
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
//#include <jimmy/jimmy_servo.h>

BioloidController bioloid = BioloidController(1000000);
int data_field;
int id;

ros::NodeHandle node_handle;

std_msgs::String str_msg;
ros::Publisher info_stream("info_stream", &str_msg);

void right_wave(){
  delay(100);
  bioloid.readPose();
  bioloid.playSeq(r_wave);
  while (bioloid.playing){
  bioloid.play();
  }
  delay(100);
}    


void message_callback( const std_msgs::Int16& servo_msg){
  data_field = servo_msg.data;
  //digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  right_wave();
}


ros::Subscriber<std_msgs::Int16> sub("single_servo", &message_callback); //only one subscriber in this script, ideally we should be able to create multiple ones


void setup(){
  pinMode(0,OUTPUT); //output pin for the dynamixels
  //pinMode(13, OUTPUT); //toggle LED on pin 1 of arduino when message has been received   

  bioloid.loadPose(rest);   // load the pose from FLASH, into the nextPose buffer
  bioloid.readPose();            // read in current servo positions to the curPose buffer
  bioloid.interpolateSetup(500); // setup for interpolation from current->next over 1/2 a second
  while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
      bioloid.interpolateStep();     // move servos, if necessary. 
      delay(3);
  }

  node_handle.initNode();
  node_handle.subscribe(sub);
  node_handle.advertise(info_stream);
}

void loop(){
  node_handle.spinOnce();
  delay(1000); //delay for a second
}

