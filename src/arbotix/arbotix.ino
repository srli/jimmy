#include <ax12.h>
#include "/home/sfeng/groovy_workspace/jimmy/include/jimmy/arbotix_comm.h"

int led = 0;
int servo = 1;
int pos = 0;
int ang = 0;
int idx = 0;

ArbotixCommData dtc, dfc;        // data to and from computer
uint8_t in_buf[200];
uint8_t out_buf[200];

void send_int(int16_t num)
{
  uint8_t in_buf[sizeof(int16_t)];
  memcpy(in_buf, &num, sizeof(int16_t));
  Serial.write(in_buf, sizeof(int16_t));
}

void reset() {
  pos = 0;
  ang = 0;
  SetPosition(servo, pos);
}

void setup() {
  Serial.begin(115200);
  reset();
}

void packData2Computer()
{
  dtc.cmd = ArbotixCommData::GetJointAngle;
  for (int i = 0; i < NUM_JOINTS; i++)
    dtc.joints[i] = GetPosition(1);
    
  dtc.genCheckSum();
  memcpy(out_buf, &dtc, sizeof(ArbotixCommData));
}

int clamp(int num, int low, int high) {
  if (num < low)
    num = low;
  if (num > high)
    num = high;
  return num;
}

int procDataFromComputer()
{
  while(Serial.available() > 0) {
    // get rid of random bits
    if (idx == 0 && Serial.peek() != ARBOTIX_START_FLAG) {
      Serial.read();
      continue;
    }
    
    in_buf[idx] = Serial.read();
    idx++;
    if (idx == sizeof(ArbotixCommData)) {
      memcpy(&dfc, in_buf, sizeof(ArbotixCommData));
      idx = 0;
      return 1;
    }
  }
  return 0;
}

int dir = 1;

int ctr = 0;

unsigned long t0, t1;
int state = 0;

void loop() 
{  
  //packData2Computer();
  //Serial.write(out_buf, sizeof(ArbotixCommData));
    
  if (procDataFromComputer()) {
    t0 = millis();
    state = 1;
    
    memcpy(in_buf, &dfc, sizeof(ArbotixCommData));
    
    dtc.joints[ctr] = dfc.seq_id;
    if (ctr < NUM_JOINTS-1)
      ctr++;
    //SetPosition(1, clamp(dfc.joints[0], -500, 500));
    
    digitalWrite(led, HIGH);
    //Serial.write(out_buf, sizeof(ArbotixCommData));
  }
  else {
    digitalWrite(led, LOW);
  }
  
  t1 = millis();
  // reset
  if (t1 - t0 > 1000) {
    // send resutls
    if (state != 0) {
      dtc.cmd = ArbotixCommData::GetJointAngle;
      dtc.seq_id = 0;
      dtc.genCheckSum();
      memcpy(out_buf, &dtc, sizeof(ArbotixCommData));
      Serial.write(out_buf, sizeof(ArbotixCommData));
    }
    state = 0;
  }
  
  /*
  if (dir == 1) {
    ang -= 10;
    if (ang < -1000)
      dir = 0;
  }
  if (dir == 0) {
    ang += 10;
    if (ang > 1000)
      dir = 1;
  }
    
  SetPosition(1, ang);//set the position of servo #1 to the current value of 'i'
  */
 
  /*  
  // get a full command
  if (procDataFromComputer()) {    
    memcpy(in_buf, &dfc, sizeof(ArbotixCommData));
    
    SetPosition(1, clamp(dfc.joints[0], -500, 500));
    
    digitalWrite(led, HIGH-digitalRead(led));
    delay(50);
    digitalWrite(led, HIGH-digitalRead(led));
    
    Serial.write(out_buf, sizeof(ArbotixCommData));
  }
  else {
    digitalWrite(led, LOW);
  }
  */
   
  /*
  if (Serial.available() >= sizeof(ArbotixCommData)) {

    digitalWrite(led, HIGH-digitalRead(led));
    
    if (sizeof(ArbotixCommData) != Serial.readBytes((char *)in_buf, sizeof(ArbotixCommData)))
    {
      digitalWrite(led, HIGH-digitalRead(led));
      delay(50);
      digitalWrite(led, HIGH-digitalRead(led));
    }
    memcpy(&dfc, in_buf, sizeof(ArbotixCommData));
    
    dfc.joints[0] += 100;
    pos += 100;
    
    memcpy(in_buf, &dfc, sizeof(ArbotixCommData));
    Serial.write(in_buf, sizeof(ArbotixCommData));
    // error parsing command
//    if (!dfc.validate())
//      return;
  
    SetPosition(1, clamp(pos, -500, 500));
//    for (int i = 0; i < NUM_JOINTS; i++)
//      SetPosition(1, clamp(dfc.joints[i], -500, 500));
  }
  */
  
  //delay(10);
}

