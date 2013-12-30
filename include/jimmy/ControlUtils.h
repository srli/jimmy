#ifndef __CONTROL_UTILS_H
#define __CONTROL_UTILS_H

#include "arbotix_comm.h"
#include "JimmyCommon.h"
#include <stdint.h>
#include "dynamixel.h"

class ControlUtils 
{
  private:
    int _port;

    ArbotixCommData _d_from_r;           // from arbotix
    ArbotixCommData _d_to_r;             // to arbotix
  
    bool sendCommand();
    static const int _id[TOTAL_JOINTS]; 

  public:
    enum GAIN_TYPE {
      P_GAIN = 0,
      I_GAIN,
      D_GAIN
    };

    int16_t ticks_to[TOTAL_JOINTS];
    int16_t ticks_from[TOTAL_JOINTS];

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

    ///////////////////////
    // ERIC
    bool getJoints() { return getJoints(joints); }
    bool setJoints() { return setJoints(joints_d); }
    bool getJoints(double *);
    bool setJoints(const double *);

    bool setGains(const int8_t *, int which);
    bool getGains(int8_t *, int which);
    bool getLoads(double *);
    ///////////////////////
    
    /*
    bool waitForReady();
    bool requestJoints();

    inline bool sendJoints_d() { return sendJoints_d(joints_d); }
    inline bool sendStandPrep() { return sendStandPrep(joints_d); }
    bool sendJoints_d(const double *j_d);
    bool sendStandPrep(const double *j_d);
    */

    static int16_t rad2tick(double r, int j);
    static double tick2rad(int16_t t, int j);
};

#endif
