#include <ax12.h>
#include "/home/sfeng/groovy_workspace/jimmy/include/jimmy/arbotix_comm.h"

int led = 0;
int pos = 0;
int ang = 0;
int idx = 0;

ArbotixCommData dtc, dfc;        // data to and from computer
uint8_t in_buf[200];
uint8_t out_buf[200];

// servo ids
int servoIds[NUM_JOINTS] = {
  19, 20, 27,
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1
  };

void send_int(int16_t num)
{
  uint8_t in_buf[sizeof(int16_t)];
  memcpy(in_buf, &num, sizeof(int16_t));
  Serial.write(in_buf, sizeof(int16_t));
}

void reset() {
  for (int i = 0; i < NUM_JOINTS; i++)
    SetPosition(servoIds[i], 2048);
}

void setup() {
  Serial.begin(115200);
  reset();
}

void packData2Computer()
{
  dtc.cmd = ArbotixCommData::GetJointAngle;
  for (int i = 0; i < NUM_JOINTS; i++)
    dtc.joints[i] = GetPosition(servoIds[i]);
    
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


unsigned long t0, t1;
int state = 0;
int seq_sum = 0;

void loop() 
{  
  /*
  for (int i = 0; i < 3; i++) {
    float rad = sin(2.*3.1415*(float)millis()/1000.);
    int tick = (rad + 3.1415) * (4096. / (2. * 3.1415));
    SetPosition(servoIds[i], tick);
  }
  */
  
  
  //packData2Computer();
  //Serial.write(out_buf, sizeof(ArbotixCommData));
  
  if (procDataFromComputer()) {
    t0 = millis();
    state = 1;
    
    memcpy(&dfc, in_buf, sizeof(ArbotixCommData));
    seq_sum += dfc.seq_id;
    
    for (int i = 0; i < NUM_JOINTS; i++)   
      SetPosition(servoIds[i], dfc.joints[i]);
    
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
      //memcpy(out_buf, &dfc, sizeof(ArbotixCommData));
      //memcpy(out_buf, &dtc, sizeof(ArbotixCommData));
      
      packData2Computer();
      dtc.cmd = ArbotixCommData::GetJointAngle;
      dtc.seq_id = seq_sum;      
      dtc.genCheckSum();
      memcpy(out_buf, &dtc, sizeof(ArbotixCommData));
      
      Serial.write(out_buf, sizeof(ArbotixCommData));
    }
    state = 0;
    seq_sum = 0;
  }
}

/*
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
     
    for (int i = 0; i < NUM_JOINTS; i++)   
      SetPosition(servoIds[i], clamp(dfc.joints[i], -500, 500));
    
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
    ctr = 0;
  }
}
*/
