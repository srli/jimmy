#include "ControlUtils.h"
#include "Utils.h"
#include "arbotix_comm.h"
#include "serial.h"
#include <math.h>

#define TICK_MIN          0
#define TICK_MAX          4096
#define TICK_ZEROS        2048

ControlUtils::ControlUtils()
{
  vec_set(joints, 0., TOTAL_JOINTS);
  vec_set(jointsd, 0., TOTAL_JOINTS);
  vec_set(joints_d, 0., TOTAL_JOINTS);

  _port = open_port("/dev/ttyUSB0");
  set_comm_parameters(_port, 115200, 8, 0, 1);
}

ControlUtils::~ControlUtils()
{
  close_port(_port);  
}

bool ControlUtils::sendCommand()
{
  _d_to_r.seq_id++;
  _d_to_r.genCheckSum();
  
  int numBytes = send_data(_port, &_d_to_r, sizeof(ArbotixCommData));
  if (numBytes != sizeof(ArbotixCommData)) {
    printf("send error\n"); 
    return false;
  }
  return true;
}

bool ControlUtils::requestJoints()
{
  _d_to_r.cmd = ArbotixCommData::RequestJointAngle;
  sendCommand();

  bool get = false;
  
  while (!get) {
    int numBytes = receive_data(_port, &_d_from_r, sizeof(ArbotixCommData), true, ARBOTIX_START_FLAG);
    if (numBytes != sizeof(ArbotixCommData)) {
      printf("rec error\n");
    }
    if (!_d_from_r.validate()) {
      printf("validate error\n");
    }
    get = (numBytes == sizeof(ArbotixCommData) && _d_from_r.validate());
  }

  for (int i = 0; i < TOTAL_JOINTS; i++) {
    joints[i] = tick2rad(_d_from_r.joints[i]);
  }
  return true;
}

bool ControlUtils::waitForReady()
{
  bool get = false;
  sleep(5);
  
  while (!get) {
    int numBytes = receive_data(_port, &_d_from_r, sizeof(ArbotixCommData), true, ARBOTIX_START_FLAG);
    if (numBytes != sizeof(ArbotixCommData)) {
      printf("rec error\n");
    }
    if (!_d_from_r.validate()) {
      printf("validate error\n");
    }
    printf("n %d, is v %d, cmd %d\n", numBytes, _d_from_r.validate(), _d_from_r.cmd);
    get = (numBytes == sizeof(ArbotixCommData) && 
           _d_from_r.validate() &&
           _d_from_r.cmd == ArbotixCommData::IsReady);
  }

  for (int i = 0; i < TOTAL_JOINTS; i++) {
    joints[i] = tick2rad(_d_from_r.joints[i]);
  }
  return true;
}

bool ControlUtils::sendJoints_d(const double *j)
{
  _d_to_r.cmd = ArbotixCommData::SetJointAngle;
  for (int i = 0; i < TOTAL_JOINTS; i++)
    _d_to_r.joints[i] = rad2tick(j[i]);
  return sendCommand();
}

bool ControlUtils::sendStandPrep(const double *j)
{
  _d_to_r.cmd = ArbotixCommData::StandPrep;
  for (int i = 0; i < TOTAL_JOINTS; i++)
    _d_to_r.joints[i] = rad2tick(j[i]);
  return sendCommand();
}








int16_t ControlUtils::rad2tick(double r)
{
  clamp(r, -M_PI, M_PI);
  return (int16_t)((r + M_PI) * (4096. / (2*M_PI))); 
}

double ControlUtils::tick2rad(int16_t t)
{
  clamp(t, (int16_t)0, (int16_t)4096);
  return (double)(t - 2048) * (2*M_PI / 4096.);
}
