#include <jimmy/jimmy_command.h>
#include <jimmy/jimmy_servo.h>
#include <ros/ros.h>

int main(int argc, char **argv)
{

  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "test_send_servo_cmd", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();

  ros::Time last_ros_time_;
  bool wait = true;
  while (wait) {
    last_ros_time_ = ros::Time::now();
    if (last_ros_time_.toSec() > 0) {
      wait = false;
    }
  }

  ros::Publisher pub = rosnode.advertise<jimmy::jimmy_servo>("jimmy_move_servo", 10);
  jimmy::jimmy_servo servos;

  int type = atoi(argv[1]);
  if (type <= jimmy::jimmy_servo::ID_R_KNEE){
    printf("Bending knees!\n");
  }
  else if (type > jimmy::jimmy_servo::ID_R_KNEE){
    printf("Not bending knees\n");
  }

/*  servos.positions.push_back(1.0);
  servos.positions.push_back(0.4);
  servos.servo_names.push_back("left_elbow");
  servos.servo_names.push_back("right_elbow");

  getchar();
  pub.publish(servos);*/
  getchar();
  pub.publish(servos);

  for (int i = 0; i < 10000; i++)
    ros::spinOnce();
}
