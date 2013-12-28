#include <ax12.h>
#include "/home/sfeng/groovy_workspace/jimmy/include/jimmy/arbotix_comm.h"

#define STAND_PREP_DURATION          5000

#define ST_STAND_PREP                0
#define ST_SERVO                     1

int led = 0;

ArbotixCommData dtc, dfc;        // data to and from computer
uint8_t in_buf[200];
uint8_t out_buf[200];

// linear interp for stand prep
int joints0[NUM_JOINTS];
int joints1[NUM_JOINTS];
unsigned long t0, t1;
unsigned long stateTime0, stateTime;
unsigned long lastCMDtime;

int state = 0;

// coms
int msgIdx = 0;
int seq_sum = 0;

// servo ids
int servoIds[NUM_JOINTS] = {
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 20, 27
};

void reset() {
  state = ST_STAND_PREP;
  stateTime0 = millis();
  lastCMDtime = 0;

  msgIdx = 0;
  seq_sum = 0;

  setupStandPrep();
}

void setupStandPrep()
{
  for (int i = 0; i < NUM_JOINTS; i++) {
    joints0[i] = GetPosition(servoIds[i]);
  }
  t0 = millis();
  t1 = t0 + STAND_PREP_DURATION;
}

void interpJoints()
{
  float dt = (float)(millis() - t0) / (float)(t1 - t0);
  if (dt > 1)
    dt = 1;
  if (dt < 0)
    dt = 0;

  int d;
  for (int i = 0; i < NUM_JOINTS; i++) {
    d = joints0[i] + (int)(dt * (float)(joints1[i] - joints0[i]));
    SetPosition(servoIds[i], d);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NUM_JOINTS; i++)
    joints1[i] = 2048;

  reset();  
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
    if (msgIdx == 0 && Serial.peek() != ARBOTIX_START_FLAG) {
      Serial.read();
      continue;
    }

    in_buf[msgIdx] = Serial.read();
    msgIdx++;
    if (msgIdx == sizeof(ArbotixCommData)) {
      memcpy(&dfc, in_buf, sizeof(ArbotixCommData));
      msgIdx = 0;
      
      return dfc.validate();
    }
  }
  return 0;
}


void loop() 
{
  stateTime = millis() - stateTime0;
  
  // process command
  if (procDataFromComputer()) {
    lastCMDtime = millis();

    // parsing commands
    switch (dfc.cmd) {
      case ArbotixCommData::SetJointAngle:
        for (int i = 0; i < NUM_JOINTS; i++)
          joints1[i] = dfc.joints[i]; 
        break;

      case ArbotixCommData::RequestJointAngle:
        dtc.cmd = ArbotixCommData::GetJointAngle;
        for (int i = 0; i < NUM_JOINTS; i++)
          dtc.joints[i] = GetPosition(servoIds[i]);
      
        dtc.genCheckSum();
        memcpy(out_buf, &dtc, sizeof(ArbotixCommData));
        Serial.write(out_buf, sizeof(ArbotixCommData));
        break;

      case ArbotixCommData::StandPrep:
        for (int i = 0; i < NUM_JOINTS; i++)
          joints1[i] = dfc.joints[i];
        reset();
        break;

      default:
        break;
    }
    seq_sum += dfc.seq_id;
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
  
  switch (state) {
    case ST_STAND_PREP:
      interpJoints();
      if (stateTime > STAND_PREP_DURATION) {
        stateTime0 = millis();
        state = ST_SERVO;
  
        dtc.cmd = ArbotixCommData::IsReady;
        for (int i = 0; i < NUM_JOINTS; i++)
          dtc.joints[i] = GetPosition(servoIds[i]);
      
        dtc.genCheckSum();
        memcpy(out_buf, &dtc, sizeof(ArbotixCommData));        
        Serial.write(out_buf, sizeof(ArbotixCommData));        
      }
      break;
  
    case ST_SERVO:
      for (int i = 0; i < NUM_JOINTS; i++)
        SetPosition(servoIds[i], joints1[i]);        
      break;
  
    default:
      break;
  }
}


