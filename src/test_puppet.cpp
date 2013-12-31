#include "ControlUtils.h"
#include "Utils.h"
#include <math.h>
#include <fstream>
#include <ros/ros.h>
#include <ros/package.h>
#include "RobotState.h"
 
int main()
{
 	ControlUtils utils;
    
  std::string name = ros::package::getPath("jimmy") + "/scripts/puppte";
  std::ofstream out;
  out.open(name.c_str(), std::ofstream::out | std::ofstream::app);
	
  while(true) {
    getchar();
    
    assert(utils.getJoints());
    for (int i = 0; i < TOTAL_JOINTS; i++) {
      printf("%10s %g %d\n", RobotState::jointNames[i].c_str(), utils.joints[i], utils.ticks_from[i]);  
    }
    
    int ctr;
    for (ctr = 0; ctr < 6; ctr++)
      out << utils.joints[ctr] << " ";
    out << std::endl;
    for (; ctr < 12; ctr++)
      out << utils.joints[ctr] << " ";
    out << std::endl;
    for (; ctr < 16; ctr++)
      out << utils.joints[ctr] << " ";
    out << std::endl;
    for (; ctr < 20; ctr++)
      out << utils.joints[ctr] << " ";
    out << std::endl;
    for (; ctr < 23; ctr++)
      out << utils.joints[ctr] << " ";
    out << std::endl;
    out << std::endl;

    out.flush();
  }

}
