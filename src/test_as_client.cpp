#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/mman.h>
#include <ros/ros.h>
#include <jimmy/serial.h>
#include <jimmy/arbotix_comm.h>
#include <time.h>
#include <algorithm>
#include <jimmy/Utils.h>

inline int16_t rad2tick(double r)
{
}

inline double tick2rad(int16_t t)
{
}
 
inline double get_time() {
  struct timespec the_tp;
  clock_gettime( CLOCK_MONOTONIC, &the_tp );
  return ((double) (the_tp.tv_sec)) + 1.0e-9*the_tp.tv_nsec;  
}

// arduino int is 2 bytes

int16_t buf2int(const char *buf)
{
  int16_t ret;
  memcpy(&ret, buf, sizeof(int16_t));
  return ret;
}

bool procDataFromRobot(char *buf, int &idx)
{

  return false;
}

int main(int argc, char **argv ) 
{
  int err;
  char buf[100];

  int port = open_port("/dev/ttyUSB0");
  set_comm_parameters(port, 115200, 8, 0, 1);

  ArbotixCommData dtr, dfr;     // data_to_robot, data_from_robot;
  int numBytes;

  double t0, t1;
  double dt_acc = 0;
  int ctr;
  
  /*
  // recv speed test
  for (ctr = 0; ctr < 1000; ctr++) {
    t0 = get_time();
    numBytes = receive_data(port, &dfr, sizeof(ArbotixCommData), true, ARBOTIX_START_FLAG);
    if (numBytes != sizeof(ArbotixCommData))
      printf("rec error\n");
    assert(dfr.validate());
    
    //for (int j = 0; j < NUM_JOINTS; j++)
    //  printf("joint %d %d\n", j, dfr.joints[j]);

    t1 = get_time();

    dt_acc += (t1-t0);
  }
  printf("avg dt %g\n", dt_acc / (double)ctr);
  */

  // send speed test
  dtr.seq_id = 0;
  double T0 = get_time();
  uint8_t seqSum = 0;

  //double dt = 0.01;
  for (ctr = 0; ctr < 1000; ctr++) {
    seqSum += dtr.seq_id;

    t0 = get_time();

    dtr.joints[0] = rad2tick(sin(2*M_PI*(t0-T0)));
    dtr.joints[1] = rad2tick(sin(2*M_PI*(t0-T0)));
    dtr.joints[2] = rad2tick(sin(2*M_PI*(t0-T0)));
    dtr.genCheckSum();

    numBytes = send_data(port, &dtr, sizeof(ArbotixCommData));
    if (numBytes != sizeof(ArbotixCommData))
      printf("send error\n");
    
    dtr.seq_id++;    

    //for (int j = 0; j < NUM_JOINTS; j++)
    //  printf("joint %d %d\n", j, dfr.joints[j]);

    usleep(1e4);
    t1 = get_time();
    dt_acc += (t1-t0);
  }
  printf("done sending\n");
  sleep(2);
    
  numBytes = receive_data(port, &dfr, sizeof(ArbotixCommData), true, ARBOTIX_START_FLAG);
  if (numBytes != sizeof(ArbotixCommData))
    printf("rec error\n");
  
  for (int i = 0; i < NUM_JOINTS; i++)
    printf("%d\n", dfr.joints[i]);
  
  assert(dfr.validate());

  printf("%d %d\n", seqSum, dfr.seq_id);

  /*
  for (int i = 0; i < 10; i++) {
    if (i == 0)
      cmd = 0;
    else if (i < 5)
      cmd = 1;
    else 
      cmd = -1;

    int err = send_data(port, &cmd, 1);
    if (err != 1)
      printf("send error\n"); 
    else 
      printf("send %d\n", cmd);


    err = receive_data(port, buf, 2);
    if (err != 2)
      printf("rec error\n"); 
    else
      printf("rec %d\n", buf2int(buf));

    sleep(1);
  }
  */

  return 0;
}
