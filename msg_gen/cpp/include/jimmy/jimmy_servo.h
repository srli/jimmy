/* Auto-generated by genmsg_cpp for file /home/sophie/catkin_ws/src/jimmy/msg/jimmy_servo.msg */
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
  , positions()
  {
  }

  jimmy_servo_(const ContainerAllocator& _alloc)
  : servo_names(_alloc)
  , positions(_alloc)
  {
  }

  typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _servo_names_type;
  std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  servo_names;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _positions_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  positions;


  typedef boost::shared_ptr< ::jimmy::jimmy_servo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::jimmy::jimmy_servo_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct jimmy_servo
typedef  ::jimmy::jimmy_servo_<std::allocator<void> > jimmy_servo;

typedef boost::shared_ptr< ::jimmy::jimmy_servo> jimmy_servoPtr;
typedef boost::shared_ptr< ::jimmy::jimmy_servo const> jimmy_servoConstPtr;


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
    return "38811bf91898e543ed7e7f543da886c8";
  }

  static const char* value(const  ::jimmy::jimmy_servo_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x38811bf91898e543ULL;
  static const uint64_t static_value2 = 0xed7e7f543da886c8ULL;
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
    return "string[] servo_names\n\
float64[] positions\n\
\n\
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

