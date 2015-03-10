#include <ax12.h>
#include <BioloidController.h>
#include "poses.h"

BioloidController bioloid = BioloidController(1000000);

const int SERVOCOUNT = 11;
const int servoCount = 11;
int servoNums[servoCount] = {1,2,3,4,5,6,19,20,21,22,27};

int id;
int pos;
boolean IDCheck;
boolean RunCheck;

void setup(){
   pinMode(0,OUTPUT);  
   
   //initialize variables 
   id = 1;
   pos = 0;
   IDCheck = 1;
   RunCheck = 0;
  //open serial port
   Serial.begin(9600);
   delay (500);   
    Serial.println("###########################");    
   Serial.println("Serial Communication Established.");    

  //Check Lipo Battery Voltage
  CheckVoltage();

  MoveCenter();
  
  //Scan Servos, return position.
  //ScanServo();
  
  //MoveTest(10);
  
  MenuOptions();
 
  RunCheck = 1;
}

void loop(){
  // read the sensor:
  
    int inByte = Serial.read();
    switch (inByte) {  
    case 'a':    
      ScanServo();
      break;

    case 'b':    
      MoveCenter();
      break; 

     case 'c':    
      RelaxServos();
      break;     

    case 'd':
      MoveTest(1);
      break;
      
    case 'e':    
      CheckVoltage();
      break;

    case 'f':
      LEDTest();
      break;
     
//    default:
//    for (int thisServo = 0; thisServo < servoCount; thisServo++) {
//      if (servoNums[thisServo] == String(inByte)){
//        Serial.print("Detected servo number ");
//        Serial.println(inByte);
//        MoveTest(inByte);
//        break;
//      }
//    }
  
}
}

void ScanServo(){
  id = 1;  
  Serial.println("###########################");
  Serial.println("Starting Servo Scanning Test.");
  Serial.println("###########################");
      
  for (int thisServo = 0; thisServo < servoCount; thisServo++) {
  id = servoNums[thisServo];
  pos =  ax12GetRegister(id, 36, 2);
  Serial.print("Servo ID: ");
  Serial.println(id);
  Serial.print("Servo Position: ");
  Serial.println(pos);
  
  if (pos <= 0){
  Serial.println("###########################");
  Serial.print("ERROR! Servo ID: ");
  Serial.print(id);
  Serial.println(" not found. Please check connection and verify correct ID is set.");
  Serial.println("###########################"); 
  IDCheck = 0;
  }
  
  delay(1000);
  }
  
  if (IDCheck == 0){
    Serial.println("###########################");
    Serial.println("ERROR! Servo ID(s) are missing from Scan. Please check connection and verify correct ID is set.");
    Serial.println("###########################");  
  }
  else{
  
  Serial.println("All servo IDs present.");
  }
    if (RunCheck == 1){
    MenuOptions();
  }

}



void CheckVoltage(){  
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
    if (RunCheck == 1){
      MenuOptions();
  }
      Serial.println("###########################"); 
}

void MoveCenter(){
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
    if (RunCheck == 1){
      MenuOptions();
  }
}





void MoveTest(int input){
  Serial.println("###########################");
  Serial.println("Initializing Movement Sign Test");  
  Serial.println("###########################");
  delay(500);  
  id = input;
  pos = 2048;
  Serial.print("Moving Servo ID: ");
  Serial.println(id);  

  while(pos >= 1950){  
  SetPosition(id, pos);
  pos = pos--;
  delay(10);
  }

  Serial.println("Recentering");

  while(pos <= 2048){  
  SetPosition(id, pos);
  pos = pos++;
  delay(10);
  }
  
  if (RunCheck == 1){
   MenuOptions();
  }
  
}



void MenuOptions(){
  
    Serial.println("###########################"); 
    Serial.println("Please enter option a-f to run individual tests again.");     
    Serial.println("a) Servo Scanning Test");        
    Serial.println("b) Move Servos to Center");    
    Serial.println("c) Relax Servos");            
    Serial.println("d) Move servo 1; Comm test");                
    Serial.println("e) Check System Voltage");   
    Serial.println("f) Perform LED Test");       
    Serial.println("###########################"); 
}

void RelaxServos(){
  id = 1;
  Serial.println("###########################");
  Serial.println("Relaxing Servos.");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    Relax(id);
    id = (id++)%SERVOCOUNT;
    delay(50);
  }
   if (RunCheck == 1){
      MenuOptions();
  }
}

void LEDTest(){
    id = 1;
  Serial.println("###########################");
  Serial.println("Running LED Test");
  Serial.println("###########################");    
  while(id <= SERVOCOUNT){
    ax12SetRegister(id, 25, 1);
    Serial.print("LED ON - Servo ID: ");
    Serial.println(id);
    delay(3000);
    ax12SetRegister(id, 25, 0);  
    Serial.print("LED OFF - Servo ID: ");
    Serial.println(id);    
    delay(3000);    
    id = id++;
  }
  
   if (RunCheck == 1){
      MenuOptions();
  }
}
    
