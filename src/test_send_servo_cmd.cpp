#include <jimmy/jimmy_servo.h>
#include <ros/ros.h>

int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("argc > 2\n");
    exit(-1);
  }

  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "test_send_servo_cmd", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();

  ros::Time last_ros_time_;
  bool wait = true;
  while (wait) {
    printf("waiting!\n");
    last_ros_time_ = ros::Time::now();
    if (last_ros_time_.toSec() > 0) {
      wait = false;
    }
  }

  ros::Publisher pub = rosnode.advertise<jimmy::jimmy_servo>("jimmy_move_servo", 10);
  jimmy::jimmy_servo servos;


  //printf("%d is the servo?\n", servos);

  int servo_number = atoi(argv[1]);
  float servo_position = atof(argv[2]);

  servos.servo_numbers.push_back(servo_number);
  servos.positions.push_back(servo_position);

/*  servos.positions.push_back(1.0);
  servos.positions.push_back(0.4);
  servos.servo_names.push_back("left_elbow");
  servos.servo_names.push_back("right_elbow");*/
  //getchar();
  //sleep(5);
  sleep(2);  
  printf("publishing");
  pub.publish(servos);

  while (true) {
    ros::spinOnce();
  }
}
