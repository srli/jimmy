#include <jimmy/jimmy_servo.h>
#include <ros/ros.h>
#include <typeinfo>

int main(int argc, char **argv)
{
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
  


  for (int i = 1; i < argc; i+=2)
  {
    float servo_position = atof(argv[i+1]);
    if (typeid(argv[i]) == typeid("string")) {
      printf("adding this to servo names\n");
      servos.servo_names.push_back(argv[i]);
    }
    else {
      printf("adding this to servo numbers\n");
      int servonum = atoi(argv[i]);
      servos.servo_numbers.push_back(servonum);
    }
    servos.positions.push_back(servo_position);
  }

  printf("Press enter to send message\n");
  getchar();

  pub.publish(servos);
  printf("publishing\n");

  for (int i = 0; i < 10000; i++)
    ros::spinOnce();
}
