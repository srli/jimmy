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

ros::NodeHandle node_handle;

std_msgs::String str_msg;
ros::Publisher info_stream("info_stream", &str_msg);


void left_wave(){
  delay(100);
  bioloid.readPose();
  bioloid.playSeq(l_wave);
  while (bioloid.playing){
  bioloid.play();
  }
  delay(100);
}    


void action_callback( const std_msgs::Int16& servo_msg){
  data_field = servo_msg.data;
  //digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  left_wave();
}

//TO DO: FIX THIS IDLE CALLBACK
//not latching to messages properly, maybe wrong use of switch statements or improper interpolation
void idle_callback(const std_msgs::Int16& idle_msg){
  delay(100);
  int randint = idle_msg.data; //random(4); //generates random number we use to select random gesture
  bioloid.readPose();
  switch(randint){
  case 1:
    bioloid.loadPose(ran1);
  case 2:
    bioloid.loadPose(ran2);
  case 3:
    bioloid.loadPose(ran3);    
  case 4:
    bioloid.loadPose(con1);
  case 5:
    bioloid.loadPose(con2);
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
    bioloid.loadPose(rest);
  }
  bioloid.interpolateSetup(random(10,30)*100);
  while (bioloid.interpolating > 0){
    bioloid.interpolateStep();
    delay(3);
  }

  delay(100);
}

ros::Subscriber<std_msgs::Int16> sub_action("single_servo", &action_callback); //subscribes to action gestures
ros::Subscriber<std_msgs::Int16> sub_idle("idle", &idle_callback); //when python code detects idle, publishes idle gestures randomly

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
  node_handle.subscribe(sub_action);
  node_handle.subscribe(sub_idle);
  node_handle.advertise(info_stream);
}

void loop(){
  node_handle.spinOnce();
  delay(1000); //delay for a second
}

