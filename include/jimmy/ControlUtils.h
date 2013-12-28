#ifndef __CONTROL_UTILS_H
#define __CONTROL_UTILS_H

#include "arbotix_comm.h"
#include "JimmyCommon.h"
#include <stdint.h>

class ControlUtils 
{
  private:
    int _port;

    ArbotixCommData _d_from_r;           // from arbotix
    ArbotixCommData _d_to_r;             // to arbotix
  
    bool sendCommand();

  public:
    // from arbotix
    double joints[TOTAL_JOINTS];      // only gets filled when requested
    double jointsd[TOTAL_JOINTS];     // probably all zeros

    // to arbotix
    double joints_d[TOTAL_JOINTS];

    ControlUtils();

    bool requestJoints();    
    bool sendJoints_d();
    bool sendStandPrep();

    static int16_t rad2tick(double r);
    static double tick2rad(int16_t t);
};

#endif
