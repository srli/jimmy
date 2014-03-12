#include <jimmy/jimmy_gesture.h>
#include <ros/ros.h>

int main(int argc, char **argv)
{
// {
//   if (argc < 2) {
//     printf("argc > 2\n");
//     exit(-1);
//   }

  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "test_gesture", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();

  ros::Time last_ros_time_;
  bool wait = true;
  while (wait) {
    last_ros_time_ = ros::Time::now();
    if (last_ros_time_.toSec() > 0) {
      wait = false;
    }
  }

  ros::Publisher pub = rosnode.advertise<jimmy::jimmy_gesture>("jimmy_send_gesture", 10);
  jimmy::jimmy_gesture gestures;

  int type = atoi(argv[1]);
  
  gestures.cmd = type;
  printf("press enter to send gesture %d\n", type);

  getchar();
  pub.publish(gestures);

  printf("Gesture sent!\n");

  for (int i = 0; i < 10000; i++)
    ros::spinOnce();
}
