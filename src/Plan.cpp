#include "Plan.h"	
#include "JimmyCommon.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "IK.h"



void Plan::clearForRecord() {
	for(int i = 0; i < 3; i++) {
		nomPlanForRecord[i] = 0.0;
	}
	for(int i = 0; i < 7; i++) {
		nomNowForRecord[i] = 0.0;
	}

}

void Plan::addToLog(Logger &logger) {
	logger.add_datapoint("PN.nomPln[X]","m",&(nomPlanForRecord[0]));
	logger.add_datapoint("PN.nomPln[Y]","m",&(nomPlanForRecord[1]));
	logger.add_datapoint("PN.nomPln[YAW]","r",&(nomPlanForRecord[2]));

	logger.add_datapoint("PN.nom[X]","m",&(nomNowForRecord[0]));
	logger.add_datapoint("PN.nom[Y]","m",&(nomNowForRecord[1]));
	logger.add_datapoint("PN.nom[YAW]","r",&(nomNowForRecord[2]));
	logger.add_datapoint("PN.z_d[X]","m",&(nomNowForRecord[3]));
	logger.add_datapoint("PN.z_d[Y]","m",&(nomNowForRecord[4]));
	logger.add_datapoint("PN.com[X]","m",&(nomNowForRecord[5]));
	logger.add_datapoint("PN.com[Y]","m",&(nomNowForRecord[6]));
}


Plan::Plan() {
	MAX_SPEED = 0.02;
	MAX_ACCEL = 0.01;
	SS_TIME = 3.0;
	DS_TIME = 2.0;
	DS0_TIME = DS_TIME+SS_TIME/2.0;
	LO_TIME = 0.8;
	TD_TIME = LO_TIME;
	SWING_HEIGHT = 0.05;
	TIME_STEP = 0.01;
	STANCE_WIDTH = 0.2;
	PRE_PLAN_TIME = 5.0;
	endTime = std::numeric_limits<double>::infinity();
	startDDPind = 0;

	ddp = new LipmConstHeightPlanner(0.45, TIME_STEP);
}

Plan::Plan(const char *config) {
	static std::map <const std::string, double *> planLookup;

	planLookup["MAX_SPEED"] = &(MAX_SPEED);
	planLookup["MAX_ACCEL"] = &(MAX_ACCEL);
	planLookup["SS_TIME"] = &(SS_TIME);
	planLookup["DS_TIME"] = &(DS_TIME);
	planLookup["DS0_TIME"] = &(DS0_TIME);
	planLookup["LO_TIME"] = &(LO_TIME);
	planLookup["TD_TIME"] = &(TD_TIME);
	planLookup["SWING_HEIGHT"] = &(SWING_HEIGHT);
	planLookup["TIME_STEP"] = &(TIME_STEP);
	planLookup["STANCE_WIDTH"] = &(STANCE_WIDTH);
	planLookup["PRE_PLAN_TIME"] = &(PRE_PLAN_TIME);
	
	std::ifstream in(config);

	if (in.good()) {

		std::string name;
		std::map<const std::string, double *>::iterator res;
		double val;
		while (true) {
			in >> name;
			if (in.eof())		break;

			res = planLookup.find(name);
			// can't find item
			if (res == planLookup.end()) {
				std::cerr << "unknown param: " << name << " aborting." << std::endl;
				break;
			}

			in >> val;
			*(res->second) = val;
			std::cerr << "read " << name << " = " << val << std::endl;
		}
	}
	else {
		printf("couldn't open config file\n");
	}
	in.close();

	IKcon::timeStep = TIME_STEP;
}

void Plan::driveFutureRobot(double vForward, double vLeft, double dTheta) {
	
	//drive virtual point
	double oldYaw = nomYaw.back();
	double newYaw = oldYaw+dTheta*TIME_STEP;
	nomYaw.push_back(newYaw);
	double newX = nomP[X].back() + (vForward*cos(oldYaw) - vLeft*sin(oldYaw))*TIME_STEP;
	nomP[X].push_back(newX);
	double newY = nomP[X].back() + (vForward*sin(oldYaw) + vLeft*cos(oldYaw))*TIME_STEP;
	nomP[Y].push_back(newY);

	nomPlanForRecord[0] = newX;
	nomPlanForRecord[1] = newY;
	nomPlanForRecord[2] = newYaw;


	int nextTDind = prevTDind + (DS_TIME+SS_TIME)/TIME_STEP;
	if(allSteps.size() == 2)		nextTDind = prevTDind + (DS0_TIME+SS_TIME)/TIME_STEP;		//long first DS

	int curInd = nomYaw.size();

	//printf("drive %d %d, %g %g %g\n", curInd, nextTDind, vForward, vLeft, dTheta);
	double time = curInd*TIME_STEP;
	if(curInd >= nextTDind) {	//time for a new step
		steps[nextTD].back()->lo = time-SS_TIME;	//beginning of swing
		Step *s;
		//offset step to left or right of nominal trajectory by half a stance width
		if(nextTD == LEFT) {
			s = new Step(newX - sin(newYaw)*STANCE_WIDTH/2.0, newY + cos(newYaw)*STANCE_WIDTH/2.0, newYaw, time, nextTD);
		}
		else {	//nextTD == RIGHT
			s = new Step(newX + sin(newYaw)*STANCE_WIDTH/2.0, newY - cos(newYaw)*STANCE_WIDTH/2.0, newYaw, time, nextTD);				
		}
		addStep(s);
		prevTDind = curInd;	//keep track of when to add the next step
		nextTD = 1-nextTD;	//keep track of which foot is next
	}
}



void Plan::addStep(Step *step) {
	printf("Add step: %d, %g, %g, %g, %g\n",step->side, step->x, step->y, step->yaw, step->td);

	int s = step->side;
	if(steps[s].size() > 0) {	//if not initializing
		steps[s].back()->lo = step->td-SS_TIME;		//previous step now ends
		foot[s][X].addMove(steps[s].back()->lo, step->td, step->x);
		foot[s][Y].addMove(steps[s].back()->lo, step->td, step->y);
		foot[s][Z].addMove(steps[s].back()->lo, steps[s].back()->lo+LO_TIME, SWING_HEIGHT, Linear);		//lift foot up
		foot[s][Z].addMove(step->td-TD_TIME, step->td, SWING_HEIGHT, Linear);							//put foot down

		//beginning of SS
		zmp_d[X].addKnot(step->td-SS_TIME, allSteps.back()->x);
		zmp_d[Y].addKnot(step->td-SS_TIME, allSteps.back()->y);
		//end of SS
		zmp_d[X].addKnot(step->td, allSteps.back()->x);
		zmp_d[Y].addKnot(step->td, allSteps.back()->y);

		//fill in zmpVec
		int startInd = zmpVec[X].size();
		for(int i = startInd; i < step->td/TIME_STEP; i++) {
			for(int j = 0; j < 2; j++) {
				zmpVec[j].push_back(zmp_d[j].readPos(i*TIME_STEP));
			}
		}

		if(com[X].size() < startDDPind) {
			startDDPind = com[X].size()-1;
			printf("running extra trajectory\n");
		}

		printf("Planning from %d to %d\n", startDDPind, zmpVec[X].size());
		//run DDP in both x and y
		for(int i = 0; i < 2; i++) {
			printf("dim %d\n", i);
			double x0[2] = {(com[i])[startDDPind], (comd[i])[startDDPind]};
			printf("{%g %g}\n",x0[0], x0[1]);
			printf("%p\n", ddp);
			ddp->getTrajs(x0, startDDPind, zmpVec[i], com[i], comd[i]);
			printf("Plan %d complete \n", i);
		}

		if(com[X].size() != zmpVec[X].size()) {
			printf("Size mismatch after ddp plan %d %d\n", com[X].size(), zmpVec[X].size());
			exit(-1);
		}
	}
	else {	//if initializing
		foot[s][X].addKnot(0.0, step->x, 0, 0);
		foot[s][Y].addKnot(0.0, step->y, 0, 0);
		foot[s][Z].addKnot(0.0, 0.0, 0, 0);
	}
	steps[s].push_back(step);
	allSteps.push_back(step);
}


void Plan::initFeet(double lx, double ly, double lYaw, double rx, double ry, double rYaw) {
	clearVectors();
	Step *il = new Step(lx, ly, lYaw, -std::numeric_limits<double>::infinity(), LEFT);
	addStep(il);
	Step *ir = new Step(rx, ry, rYaw, -std::numeric_limits<double>::infinity(), RIGHT);
	addStep(ir);
	nomP[X].push_back((lx+rx)/2.0);
	nomP[Y].push_back((ly+ry)/2.0);
	nomYaw.push_back((lYaw+rYaw)/2.0);
	zmp_d[X].addKnot(0,(lx+rx)/2.0);
	zmp_d[Y].addKnot(0,(ly+ry)/2.0);
	prevTDind = 0;
	nextTD = LEFT;
	endTime = std::numeric_limits<double>::infinity();
}




void Plan::clearVectors() {
	nomP[X].clear();
	nomP[Y].clear();
	nomYaw.clear();

	steps[LEFT].clear();
	steps[RIGHT].clear();
	allSteps.clear();

	zmp_d[X].clear();
	zmp_d[Y].clear();
	for(int s = 0; s < 2; s++)	for(int i = 0; i < 3; i++)	foot[s][i].clear();
	for(int i = 0; i < 2; i++) {
		zmpVec[i].clear();
		com[i].clear();
		comd[i].clear();
	}
}

void Plan::printSteps(char *fileName) {
	std::ofstream f(fileName);
	for(int i = 0; i < allSteps.size(); i++) {
		Step *s = allSteps[i];
		f << s->x << " " << s->y << " " << s->yaw << " " << s->td << " " << s->lo << " " << s->side << std::endl;
	}


	f.close();
}

void Plan::printNom(char *fileName) {
	if(nomP[X].size() != nomP[Y].size()) {
		printf("size mismatch x = %d; y = %d\n", nomP[X].size(), nomP[Y].size());
		return;
	}
	if(nomP[X].size() != nomYaw.size()) {
		printf("size mismatch x = %d; yaw = %d\n", nomP[X].size(), nomYaw.size());
		return;
	}
	
	std::ofstream f(fileName);
	for(int i = 0; i < nomP[X].size(); i++) {
		f << TIME_STEP*i << " " << nomP[X][i] << " " << nomP[Y][i] << " " << nomYaw[i] << std::endl;
	}


	f.close();
}

void Plan::stopHere() {
	Step *prev = allSteps.back();
	double yaw = prev->yaw;
	Step *s;
	if(prev->side == LEFT) {
		s = new Step(prev->x+sin(yaw)*STANCE_WIDTH, prev->y-cos(yaw)*STANCE_WIDTH, yaw, prev->td+DS_TIME+SS_TIME, RIGHT);
	}
	else {
		s = new Step(prev->x-sin(yaw)*STANCE_WIDTH, prev->y+cos(yaw)*STANCE_WIDTH, yaw, prev->td+DS_TIME+SS_TIME, LEFT);
	}
	addStep(s);
	zmp_d[X].addKnot(s->td+DS0_TIME, (s->x+prev->x)/2.0);
	zmp_d[Y].addKnot(s->td+DS0_TIME, (s->y+prev->y)/2.0);
	
	endTime = s->td+DS0_TIME;
}

bool Plan::isDone(double walkTime) {
	return walkTime > endTime;
}

void Plan::fillIK_d(IKcmd &IK_d, double t) {
	int nowInd = (int)floor(t/TIME_STEP);
	nomNowForRecord[0] = (nomP[X])[nowInd];
	nomNowForRecord[1] = (nomP[Y])[nowInd];
	nomNowForRecord[2] = (nomYaw)[nowInd];
	nomNowForRecord[3] = (zmpVec[X])[nowInd];
	nomNowForRecord[4] = (zmpVec[Y])[nowInd];
	nomNowForRecord[5] = (com[X])[nowInd];
	nomNowForRecord[6] = (com[Y])[nowInd];
	startDDPind = (t+0.1)/TIME_STEP;
	//TODO: implement
}

