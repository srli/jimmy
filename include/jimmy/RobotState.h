#ifndef __ROBOT_STATE_DEFINED
#define __ROBOT_STATE_DEFINED

// used to make M, J, nonLin terms for QP inverse dynamics

#include "SDModel.h"
#include "JimmyCommon.h"

#include "Logger.h"
#include <eigen3/Eigen/Geometry>

class RobotState {
  public:

    static const double jointLimits[2][N_JOINTS];
    static const std::string jointNames[TOTAL_JOINTS];

    Eigen::Quaterniond rootQ;
    double rootW[3];
    double root[3];  
    double rootd[3];  
    double joints[N_JOINTS];
    double jointsd[N_JOINTS];


    double com[3];
    double comd[3];
    double foot[LR][3];
    double footd[LR][3];
    double footW[LR][3];
    Eigen::Quaterniond footQ[LR];

    SDModel *model;

    double m;

    // EIGEN uses col major by default, need to explicitly say rowmajor!!!
    Eigen::Matrix<double,N_U,6,Eigen::RowMajor> J[LR];    // foot J transpose

    Eigen::Matrix<double,N_U,3,Eigen::RowMajor> Jc;       // com J transpose 

    Eigen::Matrix<double,N_U,6,Eigen::RowMajor> Jtorso;   


    RobotState();
    void fillZeros();
    void computeSDFvars();
    double *getSDFstate(double *sdState);
    void addToLog(Logger &logger);
};




#endif
