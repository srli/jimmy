//This script subscribes to single_servo node and will perform actions regarding that servo according to inputs

#include <Arduino.h>
#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"
#include <ros.h>
#include <std_msgs/Int16.h>
//#include <jimmy/jimmy_servo.h>

BioloidController bioloid = BioloidController(1000000);
int data;
int id;

ros::NodeHandle node_handle;

void message_callback( const std_msgs::Int16& servo_msg){
  data = servo_msg.data;
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  LED_test(data);
}

ros::Subscriber<std_msgs::Int16> sub("single_servo", &message_callback); //only one subscriber in this script, ideally we should be able to create multiple ones

void setup(){
  pinMode(0,OUTPUT); //output pin for the dynamixels
  pinMode(13, OUTPUT); //toggle LED on pin 1 of arduino when message has been received   

  node_handle.initNode();
  node_handle.subscribe(sub);

  //open serial port
  Serial.begin(9600);
  delay (500);   
  Serial.println("###########################");    
  Serial.println("Serial Communication Established.");
  Check_Voltage();
  Move_Center();
}

void loop(){
  node_handle.spinOnce();
  delay(1); //delay for a second
}


void Check_Voltage(){  
   // wait, then check the voltage (LiPO safety)
  float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;
  Serial.println("###########################");   
  Serial.print ("System Voltage: ");
  Serial.print (voltage);
  Serial.println (" volts.");
  if (voltage < 10.0){
    Serial.println("Voltage levels below 10v, please charge battery.");
    while(1);
  }  
  if (voltage > 10.0){
  Serial.println("Voltage levels nominal.");
  }
      Serial.println("###########################"); 
}

void Move_Center(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Center);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    Serial.println("###########################");
    Serial.println("Moving servos to centered position");
    Serial.println("###########################");    
    delay(1000);
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
}


void LED_test(int input){
    id = input;
  Serial.println("###########################");
  Serial.println("Running LED Test");
  Serial.println("###########################");    
    ax12SetRegister(id, 25, 1);
    Serial.print("LED ON - Servo ID: ");
    Serial.println(id);
    delay(3000);
    ax12SetRegister(id, 25, 0);  
    Serial.print("LED OFF - Servo ID: ");
    Serial.println(id);    
    delay(3000);    
  }
    
