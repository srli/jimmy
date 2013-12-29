#include "ControlUtils.h"
#include "Utils.h"
#include <math.h>

int main()
{
  ControlUtils utils;
  double t0, t1;
  double dt_acc = 0;
  
  double joints_d[TOTAL_JOINTS] = {0};
  for (int i = 0; i < TOTAL_JOINTS; i++)
    joints_d[i] = standPrepPose[i];

  utils.sendStandPrep(joints_d);
  utils.waitForReady();

  double T0 = get_time();
  for (int ctr = 0; ctr < 345; ctr++) {
    t0 = get_time();

    joints_d[NECK_YAW] = 1; //sin(2*M_PI*(t0-T0));
    joints_d[NECK_1] = 1; //sin(2*M_PI*(t0-T0));
    joints_d[NECK_2] = 1; //sin(2*M_PI*(t0-T0));

    utils.sendJoints_d(joints_d);
    
    usleep(1e4);
    t1 = get_time();
    dt_acc += (t1-t0);
  }
  printf("done sending\n");
   
  utils.requestJoints();
  for (int i = 0; i < TOTAL_JOINTS; i++) {
    printf("%g\n", utils.joints[i]);  
  }
}
