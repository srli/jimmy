#include "ControlUtils.h"
#include "Utils.h"
#include <math.h>

int main()
{
  ControlUtils utils;
  double t0, t1;
  double T0 = get_time();
  double dt_acc = 0;

  for (int ctr = 0; ctr < 1000; ctr++) {
    t0 = get_time();

    utils.joints_d[0] = sin(2*M_PI*(t0-T0));
    utils.joints_d[1] = sin(2*M_PI*(t0-T0));
    utils.joints_d[2] = sin(2*M_PI*(t0-T0));

    utils.sendJoints_d();
    
    usleep(1e4);
    t1 = get_time();
    dt_acc += (t1-t0);
  }
  printf("done sending\n");
  sleep(2);
   
  utils.requestJoints();
  for (int i = 0; i < TOTAL_JOINTS; i++) {
    printf("%g\n", utils.joints[i]);  
  }
}
