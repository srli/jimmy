//This script subscribes to single_servo node and will perform actions regarding that servo according to inputs

#include <Arduino.h>
#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"

#include <ros.h>
#define USE_USBCON

#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include <jimmy/jimmy_servo_nums.h>


BioloidController bioloid = BioloidController(1000000);
int data;
int id;

ros::NodeHandle node_handle;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";
char received[13] = "mreceived";
char badvoltage[13] = "bad volts";

boolean start = true;

void message_callback( const std_msgs::Int16& servo_msg){
//  Serial.println("message received!");
  data = servo_msg.data;
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  str_msg.data = received;
  chatter.publish(&str_msg);
  
  LED_test(data);
}

ros::Subscriber<std_msgs::Int16> sub("single_servo", message_callback); //only one subscriber in this script, ideally we should be able to create multiple ones

void setup(){
  //Cannot run any functions inside setup for ROSSERIAL
  
  pinMode(0,OUTPUT); //output pin for the dynamixels
  pinMode(13, OUTPUT); //toggle LED on pin 1 of arduino when message has been received   

  node_handle.initNode();
  node_handle.advertise(chatter);
  node_handle.subscribe(sub);

}

void loop(){
  //Moved setup scripts into loop.TODO: Decide if necessary
  if (start){
  Check_Voltage();
  Move_Center();
  }  
  
  node_handle.spinOnce();
  start = false;
  delay(500); //delay for a bit
}


void Check_Voltage(){  
   // wait, then check the voltage (LiPO safety)
  float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;
  if (voltage < 10.0){
    str_msg.data = badvoltage;
    chatter.publish(&str_msg);
    while(1);
  }  
 }

void Move_Center(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Center);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    delay(1000);
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
}


void LED_test(int input){
  str_msg.data = hello;
  chatter.publish(&str_msg);
  id = input;
    ax12SetRegister(id, 25, 1);
    Serial.println(id);
    delay(3000);
    ax12SetRegister(id, 25, 0);  
    Serial.println(id);    
    delay(3000);    
  }
    

