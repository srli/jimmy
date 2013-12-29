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

#define ID_R_SHOULDER_PITCH      1
#define ID_L_SHOULDER_PITCH      2
#define ID_R_SHOULDER_ROLL       3
#define ID_L_SHOULDER_ROLL       4
#define ID_R_ELBOW               5
#define ID_L_ELBOW               6
#define ID_R_HIP_YAW             7
#define ID_L_HIP_YAW             8
#define ID_R_HIP_ROLL            9
#define ID_L_HIP_ROLL            10
#define ID_R_HIP_PITCH           11
#define ID_L_HIP_PITCH           12
#define ID_R_KNEE                13
#define ID_L_KNEE                14
#define ID_R_ANKLE_PITCH         15
#define ID_L_ANKLE_PITCH         16
#define ID_R_ANKLE_ROLL          17
#define ID_L_ANKLE_ROLL          18
#define ID_HEAD_PAN              19
#define ID_HEAD_TILT             20
#define ID_R_ELBOW_YAW           21
#define ID_L_ELBOW_YAW           22
#define ID_R_WRIST_YAW           23
#define ID_L_WRIST_YAW           24
#define ID_R_GRIPPER             25
#define ID_L_GRIPPER             26
#define ID_HEAD_TILT_2           27

// servo ids
int servoIds[NUM_JOINTS] = {
  ID_L_HIP_YAW, ID_L_HIP_PITCH, ID_L_HIP_ROLL, ID_L_KNEE, ID_L_ANKLE_PITCH, ID_L_ANKLE_ROLL,
  ID_R_HIP_YAW, ID_R_HIP_PITCH, ID_R_HIP_ROLL, ID_R_KNEE, ID_R_ANKLE_PITCH, ID_R_ANKLE_ROLL,
  ID_L_SHOULDER_PITCH, ID_L_SHOULDER_ROLL, ID_L_ELBOW_YAW, ID_L_ELBOW, 
  ID_R_SHOULDER_PITCH, ID_R_SHOULDER_ROLL, ID_R_ELBOW_YAW, ID_R_ELBOW, 
  ID_HEAD_PAN, ID_HEAD_TILT, ID_HEAD_TILT_2
};

int skip[NUM_JOINTS] = {
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
  0, 0, 0
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
    if (skip[i])
      continue;
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
    if (skip[i])
      continue;
    d = joints0[i] + (int)(dt * (float)(joints1[i] - joints0[i]));
    SetPosition(servoIds[i], d);
  }
}

void setup() {
  Serial.begin(115200);
  ax12Init(1000000);
  
  delay(500);
  for (int i = 0; i < NUM_JOINTS; i++) {
    if (skip[i])
      continue;
    joints1[i] = GetPosition(servoIds[i]);;
  }

  reset();  
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
        for (int i = 0; i < NUM_JOINTS; i++) {
          if (skip[i]) {
            dtc.joints[i] = -2;
          }
          else
            dtc.joints[i] = GetPosition(servoIds[i]);
        }
      
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
  
  // actual control
  stateTime = millis() - stateTime0;    
  switch (state) {
    case ST_STAND_PREP:
      interpJoints();
      if (stateTime > STAND_PREP_DURATION) {
        stateTime0 = millis();
        state = ST_SERVO;
  
        dtc.cmd = ArbotixCommData::IsReady;
        for (int i = 0; i < NUM_JOINTS; i++) {
          if (skip[i])
            continue;
          dtc.joints[i] = GetPosition(servoIds[i]);
        }
        
      
        dtc.genCheckSum();
        memcpy(out_buf, &dtc, sizeof(ArbotixCommData));        
        Serial.write(out_buf, sizeof(ArbotixCommData));        
      }
      break;
  
    case ST_SERVO:
      for (int i = 0; i < NUM_JOINTS; i++) {
        if (skip[i])
          continue;
        SetPosition(servoIds[i], joints1[i]);        
      }
      break;
  
    default:
      break;
  }
  
}


