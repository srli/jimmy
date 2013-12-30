#ifndef __CONTROL_UTILS_H
#define __CONTROL_UTILS_H

#include "arbotix_comm.h"
#include "JimmyCommon.h"
#include <stdint.h>
#include "dynamixel.h"
#include <vector>

class ControlUtils 
{
  private:
    int _port;

    ArbotixCommData _d_from_r;           // from arbotix
    ArbotixCommData _d_to_r;             // to arbotix
  
    bool sendCommand();
    static const int _id[TOTAL_JOINTS]; 
  
    int _legIdx;

    bool syncWriteByte(int cmd, const std::vector<int8_t> &id, const std::vector<int8_t> &val);
    bool syncWriteWord(int cmd, const std::vector<int8_t> &id, const std::vector<int16_t> &val);

  public:
    enum GAIN_TYPE {
      P_GAIN = 0,
      I_GAIN,
      D_GAIN,
      THERMAL_MAX
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

    // ERIC
    bool getJoints() { return getJoints(joints); }
    bool setJoints() { return setJoints(joints_d); }
    
    bool getLegJointsCircular(double j[TOTAL_JOINTS]);
    bool getJoints(double j[TOTAL_JOINTS]);
    bool setJoints(const double j[TOTAL_JOINTS]);

    bool setGains(const int8_t gain[TOTAL_JOINTS], int type);
    bool getGains(int8_t gain[TOTAL_JOINTS], int type);
    bool getLoads(double trq[TOTAL_JOINTS]);

    bool setGain(int8_t gain, int type, int idx);
    bool getGain(int8_t *gain, int type, int idx);

    bool setStanceGain(int side);

    static int16_t rad2tick(double r, int j);
    static double tick2rad(int16_t t, int j);
};

#endif
