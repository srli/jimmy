#ifndef __ARBOTIX_COMM_H
#define __ARBOTIX_COMM_H

#include <stdlib.h>

#define NUM_JOINTS    23
#define ARBOTIX_START_FLAG      0x34

class ArbotixCommData 
{
  public:
    enum CMDType {
      SetJointAngle = 0,
      GetJointAngle
    };

    uint8_t start;
    uint8_t cmd;
    int16_t joints[NUM_JOINTS];
    uint8_t checkSum;
    uint8_t pad;

    ArbotixCommData() 
    {
      start = ARBOTIX_START_FLAG;
      pad = 0;
      cmd = checkSum = 0;
      memset(joints, 0, NUM_JOINTS*sizeof(int16_t));
    }
    
    void genCheckSum()
    {
      checkSum = cmd;
      for (int i = 0; i < NUM_JOINTS; i++) {
        checkSum += (uint8_t)(joints[i] >> 8) + (uint8_t)(joints[i]);
      }
    }

    bool validate() const
    {
      uint8_t val = cmd;
      for (int i = 0; i < NUM_JOINTS; i++) {
        val += (uint8_t)(joints[i] >> 8) + (uint8_t)(joints[i]);
      }
      return (val == checkSum);
    }
};

#endif
