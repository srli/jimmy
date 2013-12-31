#ifndef __CONTROL_UTILS_H
#define __CONTROL_UTILS_H

#include "JimmyCommon.h"
#include <stdint.h>
#include "dynamixel.h"
#include <vector>

#define ADDR_GOAL_POSITION_L	       30
#define ADDR_GOAL_POSITION_H	       31
#define ADDR_PRESENT_POSITION_L      36
#define ADDR_PRESENT_POSITION_H	     37
#define ADDR_GOAL_SPEED_L		         32
#define ADDR_GOAL_SPEED_H		         33
#define ADDR_TRQ_LIM_L               34
#define ADDR_TRQ_LIM_H               35

#define ADDR_P_GAIN                  28
#define ADDR_I_GAIN                  27
#define ADDR_D_GAIN                  26
#define ADDR_THERMAL_MAX             11
#define ADDR_PRESENT_LOAD_L          40
#define ADDR_PRESENT_LOAD_H          41
#define ADDR_PRESENT_TEMPERATURE     43
 
class ControlUtils 
{
  private:
    bool sendCommand();
    static const int _id[TOTAL_JOINTS]; 
  
    int _legIdx;

    bool syncWriteByte(int8_t addr, const std::vector<int> &joints, const std::vector<int8_t> &val);
    bool syncWriteWord(int8_t addr, const std::vector<int> &joints, const std::vector<int16_t> &val);
    bool setByte(int8_t gain, int8_t addr, int joint);
    bool getByte(int8_t *gain, int8_t addr, int joint);
    bool setAllBytes(const int8_t val[TOTAL_JOINTS], int8_t addr);
    bool getAllBytes(int8_t val[TOTAL_JOINTS], int8_t addr);

  public:
    enum CMD_TYPE {
      D_GAIN = 0,
      I_GAIN,
      P_GAIN,
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

    inline bool setPGain(int8_t gain, int joint) { return setByte(gain, ADDR_P_GAIN, joint); }
    inline bool getPGain(int8_t *gain, int joint) { return getByte(gain, ADDR_P_GAIN, joint); }
    inline bool setIGain(int8_t gain, int joint) { return setByte(gain, ADDR_I_GAIN, joint); }
    inline bool getIGain(int8_t *gain, int joint) { return getByte(gain, ADDR_I_GAIN, joint); }
    inline bool setDGain(int8_t gain, int joint) { return setByte(gain, ADDR_D_GAIN, joint); }
    inline bool getDGain(int8_t *gain, int joint) { return getByte(gain, ADDR_D_GAIN, joint); }

    inline bool setPGains(const int8_t gains[TOTAL_JOINTS]) { return setAllBytes(gains, ADDR_P_GAIN); }
    inline bool setIGains(const int8_t gains[TOTAL_JOINTS]) { return setAllBytes(gains, ADDR_I_GAIN); }
    inline bool setDGains(const int8_t gains[TOTAL_JOINTS]) { return setAllBytes(gains, ADDR_D_GAIN); }
    inline bool setThermalMax(const int8_t m[TOTAL_JOINTS]) { return setAllBytes(m, ADDR_THERMAL_MAX); }
    
    inline bool getPGains(int8_t gains[TOTAL_JOINTS]) { return getAllBytes(gains, ADDR_P_GAIN); }
    inline bool getIGains(int8_t gains[TOTAL_JOINTS]) { return getAllBytes(gains, ADDR_I_GAIN); }
    inline bool getDGains(int8_t gains[TOTAL_JOINTS]) { return getAllBytes(gains, ADDR_D_GAIN); }
    inline bool getThermalMax(int8_t m[TOTAL_JOINTS]) { return getAllBytes(m, ADDR_THERMAL_MAX); }
    inline bool getCurTemperature(int8_t t[TOTAL_JOINTS]) { return getAllBytes(t, ADDR_PRESENT_TEMPERATURE); }

    bool setTorqueLimit(int16_t lim, int joint);

    bool setStanceGain(int side);

    static int16_t rad2tick(double r, int j);
    static double tick2rad(int16_t t, int j);
};

#endif
