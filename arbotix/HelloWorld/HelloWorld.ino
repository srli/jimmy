/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>

std_msgs::Int16 str_msg;

ros::NodeHandle  nh;

ros::Publisher chatter("chatter", &str_msg);

void message_callback(const std_msgs::Int16& servo_msg){
  str_msg.data=servo_msg.data;
  chatter.publish(&str_msg);
}



ros::Subscriber<std_msgs::Int16> sub("single_servo", &message_callback);

char hello[13] = "hello world!";

void setup()
{
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);
}

void loop()
{
  //str_msg.data = hello;
  //chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
}
