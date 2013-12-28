#include "RobotState.h"
#include "Jimmy.h"
#include "SDModel_common.h"
#include "sdlib.h"

const double RobotState::jointLimits[2][N_JOINTS] = 
{
	{
		-0.43,-0.7,-0.38,0,-0.78,-0.78,
		-0.43,-0.7,-0.27,0,-0.78,-0.78,
		-1.04,-100,-100,-100,
		-1.04,-100,-100,-100
	},
	{
		0.43,0.7,0.27,1.12,0.61,0.78,
		0.43,0.7,0.38,1.12,0.61,0.78,
		1.04,100,100,100,
		1.04,100,100,100
	}
};

void RobotState::fillZeros() {
	for(int i = 0; i < 3; i++) {
		com[i] = 0.0;
		comd[i] = 0.0;
		root[i] = 0.0;
		rootd[i] = 0.0;
		rootW[i] = 0.0;
		for(int s = 0; s < 2; s++) {
			foot[s][i] = 0.0;
			footd[s][i] = 0.0;
			footW[s][i] = 0.0;
		}
	}

	rootQ = Eigen::Quaterniond::Identity();
	for(int s = 0; s < 2; s++)		footQ[s] = Eigen::Quaterniond::Identity();

	for(int i = 0; i < N_JOINTS; i++) {
		joints[i] = 0.0;
		jointsd[i] = 0.0;
	}

	m = 0.0;

	for(int s = 0; s < 2; s++)		J[s].setZero();
	Jc.setZero();
	Jtorso.setZero();
}

RobotState::RobotState() {
	model = new Jimmy();
	model->sdinit();
	fillZeros();
	computeSDFvars();
	root[Z] = -(foot[LEFT][Z]+foot[RIGHT][Z])/2.0;
	computeSDFvars();
}

double *RobotState::getSDFstate(double *sdState) {
	// copy joint pos and vel
	memcpy(sdState+6, joints, sizeof(double)*N_JOINTS);
	memcpy(sdState+N_Q+6, jointsd, sizeof(double)*N_JOINTS);

	// get position and vel
	memcpy(sdState, root, sizeof(double)*XYZ);
	sdState[3] = rootQ.x();
	sdState[4] = rootQ.y();
	sdState[5] = rootQ.z();
	sdState[26] = rootQ.w();

	memcpy(sdState+N_Q, rootd, sizeof(double)*XYZ);
	memcpy(sdState+N_Q+3, rootW, sizeof(double)*XYZ);

	return sdState;
}

void RobotState::computeSDFvars() {
	double sdfast_state[N_U+N_Q];

	// copy joint pos and vel
	memcpy(sdfast_state+6, joints, sizeof(double)*N_JOINTS);
	memcpy(sdfast_state+N_Q+6, jointsd, sizeof(double)*N_JOINTS);

	// get position and vel
	memcpy(sdfast_state, root, sizeof(double)*XYZ);
	sdfast_state[3] = rootQ.x();
	sdfast_state[4] = rootQ.y();
	sdfast_state[5] = rootQ.z();
	sdfast_state[26] = rootQ.w();

	memcpy(sdfast_state+N_Q, rootd, sizeof(double)*XYZ);
	memcpy(sdfast_state+N_Q+3, rootW, sizeof(double)*XYZ);

	set_state(model, sdfast_state);


	const int footBody[2] = {6, 12};
	double ankle2foot[3] = {0.002, 0.000, -0.094};



	double dircos[3][3];
	double qq[4];
	double com_to_ankle[3];

	double temp[6];
	for (int side = 0; side < 2; side++) {
		model->sdgetbtj(footBody[side], com_to_ankle);

		//foot positions and jacobians
		sdvadd(com_to_ankle, ankle2foot, temp);
		body_position(model, NULL, footBody[side], temp, &(foot[side][0]), J[side].data());
		model->sdvel(footBody[side], temp, &(footd[side][0]));

		//foot quaternion
		model->sdorient(footBody[side], dircos);
		sddc2quat(dircos, qq, qq+1, qq+2, qq+3);
		footQ[side] = Eigen::Quaterniond(qq[3], qq[0], qq[1], qq[2]).normalized();


		//foot angular velocities
		model->sdangvel(footBody[side], footW[side]);
	}


	double mom[3], angMom[3], KE;
	model->sdmom(mom, angMom, &KE);
	double Itmp[3][3];
	//mass, com position, and total moment of inertia
	model->sdsys(&m, com, Itmp);

	//comd from momentum
	for(int i = 0; i < 3; i++)
		comd[i] = mom[i]/m;		//compute comd from momentum

	// utorso orientation
	double zeros[3] = {0,0,0};
	body_position(model, NULL, 0, zeros, temp, Jtorso.data());

	//com jacobian
	com_jacobian(model, Jc.data());
}

void RobotState::addToLog(Logger &logger) {

}
