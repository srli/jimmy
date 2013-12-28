/*
* =====================================================================================
*
*       Filename:  ik_controller.hpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  10/02/2013 04:19:56 PM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  YOUR NAME (), 
*        Company:  
*
* =====================================================================================
*/

#ifndef __IK_CONTROLLER_H
#define __IK_CONTROLLER_H

#include "RobotState.h"
#include "IKcmd.h"
#include <eigen3/Eigen/Core>
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm> 
#include "JimmyCommon.h"

class IKcon 
{
public:
	RobotState ikrs;

	static const int MAX_ROWS = 400;
	static const int MAX_VARS = N_U;
	Eigen::Matrix<double,MAX_ROWS,MAX_VARS> _A;
	Eigen::Matrix<double,MAX_ROWS,1> _b;

	static double timeStep;

	Eigen::Matrix<double,N_U,1> _qdOld;


	std::map <const std::string, double *> lookup;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW		//weird Siyuan magic

	double IK_FOOT_WEIGHT;
	double IK_COMxyz_WEIGHT[3];
	double IK_TORSO_WEIGHT;
	double IK_REG_WEIGHT;
	double IK_DQDOT_WEIGHT;
	double IK_ARM_WEIGHT;

	double IK_ARM_RATE;
	double IK_POS_RATE;

	double QPval;

	




	bool IK(const IKcmd &cmd, double *theta_d=NULL, double *thetad_d=NULL);
	void setPose(const double *theta, const double *thetad);
	void setRoot(const double *root, Eigen::Quaterniond rootQ, const double *rootd=NULL, const double *rootW=NULL);
	void getCommand(double *theta_d, double *thetad_d);
	void addToLog(Logger &logger);

	IKcon() : 
		IK_FOOT_WEIGHT(1.0),
		IK_TORSO_WEIGHT(1.0),
		IK_REG_WEIGHT(0.1),
		IK_DQDOT_WEIGHT(0),
		IK_ARM_WEIGHT(1.0),
		IK_ARM_RATE(10.0),
		IK_POS_RATE(2.0),
		QPval(0.0)
	{ 
		for(int i = 0; i < 3; i++)	IK_COMxyz_WEIGHT[i] = 1.0;

		lookup["IK_FOOT_WEIGHT"] = &IK_FOOT_WEIGHT;
		lookup["IK_COMX_WEIGHT"] = &(IK_COMxyz_WEIGHT[X]);
		lookup["IK_COMY_WEIGHT"] = &(IK_COMxyz_WEIGHT[Y]);
		lookup["IK_COMZ_WEIGHT"] = &(IK_COMxyz_WEIGHT[Z]);
		lookup["IK_TORSO_WEIGHT"] = &IK_TORSO_WEIGHT;
		lookup["IK_REG_WEIGHT"] = &IK_REG_WEIGHT;
		lookup["IK_ARM_WEIGHT"] = &IK_ARM_WEIGHT;
		lookup["IK_DQDOT_WEIGHT"] = &IK_DQDOT_WEIGHT;
		lookup["IK_POS_RATE"] = &IK_POS_RATE;
		lookup["IK_ARM_RATE"] = &IK_ARM_RATE;

		_qdOld.setZero();
	}


	bool readParams(const char *fileName)
	{
		std::ifstream in(fileName);
		
		if (!in.good())
			return false;

		bool ret = true;
		std::string name;
		std::map<const std::string, double *>::iterator res;
		double val;
		while (true)
		{
			in >> name;
			if (in.eof())
				break;

			res = lookup.find(name);
			// can't find item
			if (res == lookup.end()) {
				std::cerr << "cant find param: " << name << std::endl;
				ret = false;
				break;
			}

			in >> val; 
			*(res->second) = val;
			//std::cerr << "read " << name << " = " << val << std::endl;
		}

		printParams();

		return ret;
	}

	void printParams() 
	{
		std::cout << "===================================\n" << "ikCon params\n";
		for (std::map<const std::string, double *>::iterator it = lookup.begin(); it != lookup.end(); it++)
			std::cout << it->first << ": " << *(it->second) << std::endl;
		std::cout << "===================================\n"; 
	}
};







#endif
