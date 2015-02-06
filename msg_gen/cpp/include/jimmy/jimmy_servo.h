/* Auto-generated by genmsg_cpp for file /home/sophie/ROS/src/jimmy/msg/jimmy_servo.msg */
#ifndef JIMMY_MESSAGE_JIMMY_SERVO_H
#define JIMMY_MESSAGE_JIMMY_SERVO_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"


namespace jimmy
{
template <class ContainerAllocator>
struct jimmy_servo_ {
  typedef jimmy_servo_<ContainerAllocator> Type;

  jimmy_servo_()
  : servo_names()
  , servo_numbers()
  , positions()
  {
  }

  jimmy_servo_(const ContainerAllocator& _alloc)
  : servo_names(_alloc)
  , servo_numbers(_alloc)
  , positions(_alloc)
  {
  }

  typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _servo_names_type;
  std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  servo_names;

  typedef std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  _servo_numbers_type;
  std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  servo_numbers;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _positions_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  positions;

  enum { ID_R_SHOULDER_PITCH = 16 };
  enum { ID_L_SHOULDER_PITCH = 12 };
  enum { ID_R_SHOULDER_ROLL = 17 };
  enum { ID_L_SHOULDER_ROLL = 13 };
  enum { ID_R_ELBOW = 19 };
  enum { ID_L_ELBOW = 15 };
  enum { ID_R_HIP_YAW = 6 };
  enum { ID_L_HIP_YAW = 0 };
  enum { ID_R_HIP_ROLL = 8 };
  enum { ID_L_HIP_ROLL = 2 };
  enum { ID_R_HIP_PITCH = 7 };
  enum { ID_L_HIP_PITCH = 1 };
  enum { ID_R_KNEE = 9 };
  enum { ID_L_KNEE = 3 };
  enum { ID_R_ANKLE_PITCH = 10 };
  enum { ID_L_ANKLE_PITCH = 4 };
  enum { ID_R_ANKLE_ROLL = 11 };
  enum { ID_L_ANKLE_ROLL = 5 };
  enum { ID_HEAD_PAN = 20 };
  enum { ID_HEAD_TILT = 21 };
  enum { ID_R_ELBOW_YAW = 18 };
  enum { ID_L_ELBOW_YAW = 14 };
  enum { ID_R_WRIST_YAW = 23 };
  enum { ID_L_WRIST_YAW = 24 };
  enum { ID_R_GRIPPER = 25 };
  enum { ID_L_GRIPPER = 26 };
  enum { ID_HEAD_TILT_2 = 22 };
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_SHOULDER_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_SHOULDER_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_SHOULDER_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_SHOULDER_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_ELBOW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_ELBOW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_HIP_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_HIP_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_HIP_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_HIP_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_HIP_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_HIP_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_KNEE;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_KNEE;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_ANKLE_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_ANKLE_PITCH;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_ANKLE_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_ANKLE_ROLL;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  HEAD_PAN;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  HEAD_TILT;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_ELBOW_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_ELBOW_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_WRIST_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_WRIST_YAW;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  R_GRIPPER;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  L_GRIPPER;
  static const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  HEAD_TILT_2;

  typedef boost::shared_ptr< ::jimmy::jimmy_servo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::jimmy::jimmy_servo_<ContainerAllocator>  const> ConstPtr;
}; // struct jimmy_servo
typedef  ::jimmy::jimmy_servo_<std::allocator<void> > jimmy_servo;

typedef boost::shared_ptr< ::jimmy::jimmy_servo> jimmy_servoPtr;
typedef boost::shared_ptr< ::jimmy::jimmy_servo const> jimmy_servoConstPtr;

template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_SHOULDER_PITCH = "RightShoulderPitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_SHOULDER_PITCH = "LeftShoulderPitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_SHOULDER_ROLL = "RightShoulderRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_SHOULDER_ROLL = "LeftShoulderRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_ELBOW = "RightElbow";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_ELBOW = "LeftElbow";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_HIP_YAW = "RightHipYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_HIP_YAW = "LeftHipYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_HIP_ROLL = "RightHipRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_HIP_ROLL = "LeftHipRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_HIP_PITCH = "RightHipPitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_HIP_PITCH = "LeftHipPitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_KNEE = "RightKnee";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_KNEE = "LeftKnee";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_ANKLE_PITCH = "RightAnklePitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_ANKLE_PITCH = "LeftAnklePitch";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_ANKLE_ROLL = "RightAnkleRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_ANKLE_ROLL = "LeftAnkleRoll";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::HEAD_PAN = "HeadPan";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::HEAD_TILT = "HeadTilt";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_ELBOW_YAW = "RightElbowYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_ELBOW_YAW = "LeftElbowYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_WRIST_YAW = "RightWristYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_WRIST_YAW = "LeftWristYaw";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::R_GRIPPER = "RightGripper";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::L_GRIPPER = "LeftGripper";
template<typename ContainerAllocator> const std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  jimmy_servo_<ContainerAllocator>::HEAD_TILT_2 = "HeadTilt2";

template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::jimmy::jimmy_servo_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::jimmy::jimmy_servo_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace jimmy

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::jimmy::jimmy_servo_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::jimmy::jimmy_servo_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::jimmy::jimmy_servo_<ContainerAllocator> > {
  static const char* value() 
  {
    return "4ea57b764f702b7be0b6106982510281";
  }

  static const char* value(const  ::jimmy::jimmy_servo_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x4ea57b764f702b7bULL;
  static const uint64_t static_value2 = 0xe0b6106982510281ULL;
};

template<class ContainerAllocator>
struct DataType< ::jimmy::jimmy_servo_<ContainerAllocator> > {
  static const char* value() 
  {
    return "jimmy/jimmy_servo";
  }

  static const char* value(const  ::jimmy::jimmy_servo_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::jimmy::jimmy_servo_<ContainerAllocator> > {
  static const char* value() 
  {
    return "int32 ID_R_SHOULDER_PITCH      = 16\n\
int32 ID_L_SHOULDER_PITCH      = 12\n\
int32 ID_R_SHOULDER_ROLL       = 17\n\
int32 ID_L_SHOULDER_ROLL       = 13\n\
int32 ID_R_ELBOW               = 19\n\
int32 ID_L_ELBOW               = 15\n\
int32 ID_R_HIP_YAW             = 6\n\
int32 ID_L_HIP_YAW             = 0\n\
int32 ID_R_HIP_ROLL            = 8\n\
int32 ID_L_HIP_ROLL            = 2\n\
int32 ID_R_HIP_PITCH           = 7\n\
int32 ID_L_HIP_PITCH           = 1\n\
int32 ID_R_KNEE                = 9\n\
int32 ID_L_KNEE                = 3\n\
int32 ID_R_ANKLE_PITCH         = 10\n\
int32 ID_L_ANKLE_PITCH         = 4\n\
int32 ID_R_ANKLE_ROLL          = 11\n\
int32 ID_L_ANKLE_ROLL          = 5\n\
int32 ID_HEAD_PAN              = 20\n\
int32 ID_HEAD_TILT             = 21\n\
int32 ID_R_ELBOW_YAW           = 18\n\
int32 ID_L_ELBOW_YAW           = 14\n\
int32 ID_R_WRIST_YAW           = 23\n\
int32 ID_L_WRIST_YAW           = 24\n\
int32 ID_R_GRIPPER             = 25\n\
int32 ID_L_GRIPPER             = 26\n\
int32 ID_HEAD_TILT_2           = 22\n\
\n\
string R_SHOULDER_PITCH      = RightShoulderPitch\n\
string L_SHOULDER_PITCH      = LeftShoulderPitch\n\
string R_SHOULDER_ROLL       = RightShoulderRoll\n\
string L_SHOULDER_ROLL       = LeftShoulderRoll\n\
string R_ELBOW               = RightElbow\n\
string L_ELBOW               = LeftElbow\n\
string R_HIP_YAW             = RightHipYaw\n\
string L_HIP_YAW             = LeftHipYaw\n\
string R_HIP_ROLL            = RightHipRoll\n\
string L_HIP_ROLL            = LeftHipRoll\n\
string R_HIP_PITCH           = RightHipPitch\n\
string L_HIP_PITCH           = LeftHipPitch\n\
string R_KNEE                = RightKnee\n\
string L_KNEE                = LeftKnee\n\
string R_ANKLE_PITCH         = RightAnklePitch\n\
string L_ANKLE_PITCH         = LeftAnklePitch\n\
string R_ANKLE_ROLL          = RightAnkleRoll\n\
string L_ANKLE_ROLL          = LeftAnkleRoll\n\
string HEAD_PAN              = HeadPan\n\
string HEAD_TILT             = HeadTilt\n\
string R_ELBOW_YAW           = RightElbowYaw\n\
string L_ELBOW_YAW           = LeftElbowYaw\n\
string R_WRIST_YAW           = RightWristYaw\n\
string L_WRIST_YAW           = LeftWristYaw\n\
string R_GRIPPER             = RightGripper\n\
string L_GRIPPER             = LeftGripper\n\
string HEAD_TILT_2           = HeadTilt2\n\
\n\
string[] servo_names\n\
int32[] servo_numbers\n\
float64[] positions\n\
";
  }

  static const char* value(const  ::jimmy::jimmy_servo_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::jimmy::jimmy_servo_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.servo_names);
    stream.next(m.servo_numbers);
    stream.next(m.positions);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct jimmy_servo_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::jimmy::jimmy_servo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::jimmy::jimmy_servo_<ContainerAllocator> & v) 
  {
    s << indent << "servo_names[]" << std::endl;
    for (size_t i = 0; i < v.servo_names.size(); ++i)
    {
      s << indent << "  servo_names[" << i << "]: ";
      Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.servo_names[i]);
    }
    s << indent << "servo_numbers[]" << std::endl;
    for (size_t i = 0; i < v.servo_numbers.size(); ++i)
    {
      s << indent << "  servo_numbers[" << i << "]: ";
      Printer<int32_t>::stream(s, indent + "  ", v.servo_numbers[i]);
    }
    s << indent << "positions[]" << std::endl;
    for (size_t i = 0; i < v.positions.size(); ++i)
    {
      s << indent << "  positions[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.positions[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // JIMMY_MESSAGE_JIMMY_SERVO_H

