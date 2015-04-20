#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"
#include "jimmy_arms.h"

BioloidController bioloid = BioloidController(1000000);
idle_gestures
void setup(){
   pinMode(0,OUTPUT);  
   
   //initialize variables 
  //open serial port
   Serial.begin(9600);
   delay (500);   
    Serial.println("###########################");    
   Serial.println("Serial Communication Established.");    

    bioloid.loadPose(rest);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer
    bioloid.interpolateSetup(500); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }



  MenuOptions();
 }

void loop(){
  // read the sensor:
  
    int inByte = Serial.read();

    switch (inByte) {

    case '1':    
      right_wave();
      break;

//    case '2':    
//      MoveCenter();
//      break; 
//
//     case '3':    
//      RelaxServos();
//      break;     
//
//    case '4':
//      MoveCenter();
//      MoveTest();
//      break;
//      
//    case '5':    
//      CheckVoltage();
//      break;
//
//    case '6':
//      LEDTest();
//      break;

    } 
  
}


void right_wave(){
  delay(100);
  bioloid.readPose();
  bioloid.playSeq(r_wave);
  while (bioloid.playing){
    bioloid.play();
  }
  delay(100);
}

void random_gesture(){
  delay(100);
  randint = random(10)
  bioloid.readPose();
  bioloid.playSeq(r_wave);
  while (bioloid.playing){
    bioloid.play();
  }
  delay(100);
}


void MenuOptions(){
  
    Serial.println("###########################"); 
    Serial.println("Please enter option to run related gesture");     
    Serial.println("1) Right wave");        
    Serial.println("###########################"); 
}
