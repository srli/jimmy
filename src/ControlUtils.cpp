#include "ControlUtils.h"
#include "Utils.h"
#include "arbotix_comm.h"
#include <math.h>

#define TICK_MIN          0
#define TICK_MAX          4096
#define TICK_ZEROS        2048

#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33

bool ControlUtils::getJoints(double *a)
{
  for (int i = 0; i < TOTAL_JOINTS; i++) {
    // Read present position
    ticks_from[i] = dxl_read_word(_id[i], P_PRESENT_POSITION_L);
    int CommStatus = dxl_get_result();

    if(CommStatus == COMM_RXSUCCESS)
      a[i] = tick2rad(ticks_from[i], i);
    else
      return false;

    usleep(5000);
  }

  return true;
}

bool ControlUtils::setJoints(const double *a)
{
	int CommStatus;
  // Make syncwrite packet
  dxl_set_txpacket_id(BROADCAST_ID);
  dxl_set_txpacket_instruction(INST_SYNC_WRITE);
  dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
  dxl_set_txpacket_parameter(1, 2);
  for (int i = 0; i < TOTAL_JOINTS; i++) {
    dxl_set_txpacket_parameter(2+3*i, _id[i]);
    
    ticks_to[i] = rad2tick(a[i], i);
    
    dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(ticks_to[i]));
    dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(ticks_to[i]));
  }
  dxl_set_txpacket_length((2+1)*TOTAL_JOINTS+4);

  dxl_txrx_packet();
  CommStatus = dxl_get_result();
  if( CommStatus == COMM_RXSUCCESS )
  {
    //PrintErrorCode();
    return true;
  }
  else
  {
    //PrintCommStatus(CommStatus);
    return false;
  }
}


ControlUtils::ControlUtils()
{
  vec_set(joints, 0., TOTAL_JOINTS);
  vec_set(jointsd, 0., TOTAL_JOINTS);
  vec_set(joints_d, 0., TOTAL_JOINTS);
  
  if( dxl_initialize(0, 1) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
	  exit(-1);
  }
	else
		printf( "Succeed to open USB2Dynamixel!\n" ); 
}

ControlUtils::~ControlUtils()
{
	dxl_terminate();
}

/*
bool ControlUtils::sendCommand()
{

  return true;
}

bool ControlUtils::requestJoints()
{

  return true;
}

bool ControlUtils::waitForReady()
{
  
  return true;
}

bool ControlUtils::sendJoints_d(const double *j)
{
  
  return sendCommand();
}

bool ControlUtils::sendStandPrep(const double *j)
{
  
  return sendCommand();
}
*/







int16_t ControlUtils::rad2tick(double r, int j)
{
  int16_t tmp = (int16_t)(r*ticks_per_rad)*tick_sign[j];
  return tmp+tick_zeros[j];
}

double ControlUtils::tick2rad(int16_t t, int j)
{
  int16_t tmp = (t-tick_zeros[j])*tick_sign[j];
  return (double)tmp/ticks_per_rad;
}


const double ControlUtils::ticks_per_rad = (TICK_MAX-TICK_MIN) / (2*M_PI);

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
const int ControlUtils::_id[TOTAL_JOINTS] = {
  ID_L_HIP_YAW, ID_L_HIP_PITCH, ID_L_HIP_ROLL, ID_L_KNEE, ID_L_ANKLE_PITCH, ID_L_ANKLE_ROLL,
  ID_R_HIP_YAW, ID_R_HIP_PITCH, ID_R_HIP_ROLL, ID_R_KNEE, ID_R_ANKLE_PITCH, ID_R_ANKLE_ROLL,
  ID_L_SHOULDER_PITCH, ID_L_SHOULDER_ROLL, ID_L_ELBOW_YAW, ID_L_ELBOW, 
  ID_R_SHOULDER_PITCH, ID_R_SHOULDER_ROLL, ID_R_ELBOW_YAW, ID_R_ELBOW, 
  ID_HEAD_PAN, ID_HEAD_TILT, ID_HEAD_TILT_2
};

const int16_t ControlUtils::tick_zeros[TOTAL_JOINTS] = {
	2048, 2048, 2048, 2048, 2048, 2048,
	2048, 2048, 2048, 2048, 2048, 2048,
	2048, 2560, 1024, 1707,
	2048, 1536, 3072, 2389,
  2048, 2048, 2048
};

const int16_t ControlUtils::tick_sign[TOTAL_JOINTS] = {
	-1,  1, -1, -1,  1, -1,
	-1, -1,  1, -1,  1, -1,
	 1,  1, -1, -1,
	-1,  1, -1,  1,
   1,  1,  1
};
 
