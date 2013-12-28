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
    // conversion stuff
    static const double ticks_per_rad;
    static const int16_t tick_zeros[TOTAL_JOINTS];
    static const int16_t tick_sign[TOTAL_JOINTS];
    
    // from arbotix
    double joints[TOTAL_JOINTS];      // only gets filled when requested
    double jointsd[TOTAL_JOINTS];     // probably all zeros

    // to arbotix
    double joints_d[TOTAL_JOINTS];

    ControlUtils();
    ~ControlUtils();

    bool waitForReady();
    bool requestJoints();

    inline bool sendJoints_d() { return sendJoints_d(joints_d); }
    inline bool sendStandPrep() { return sendStandPrep(joints_d); }
    bool sendJoints_d(const double *j_d);
    bool sendStandPrep(const double *j_d);

    static int16_t rad2tick(double r, int j);
    static double tick2rad(int16_t t, int j);
};

#endif
