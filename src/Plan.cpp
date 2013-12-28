#include "Plan.h"	
#include "JimmyCommon.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "IK.h"


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

	int nextTDind = prevTDind + (DS_TIME+SS_TIME)/TIME_STEP;
	if(allSteps.size() == 2)		nextTDind = prevTDind + (DS0_TIME+SS_TIME)/TIME_STEP;		//long first DS

	int curInd = nomYaw.size();
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

/*
//curveByDist returns rad/m
void Plan::genStepsFromStand(double (*curveByDist)(double p), double totDist, bool exact, bool match, double desSpeed, int firstFoot, bool decel) {
	//figure out the full speed (use MAX_SPEED as default)
	double fullSpeed = desSpeed;
	if(fullSpeed < 0)			fullSpeed = MAX_SPEED;
	if(fullSpeed > MAX_SPEED)	fullSpeed = MAX_SPEED;

	double time = 0.0;
	double speed = 0.0;
	double dist = 0.0;
	int nextSwgFoot = firstFoot;
	bool firstDS = true;
	double segmentTime = 0.0;
	while(dist <= totDist) {
		segmentTime += TIME_STEP;
		time += TIME_STEP;
		if(decel && speed > sqrt(2.0*MAX_ACCEL*(totDist-dist))) {		//decelerate at end
			speed -= MAX_ACCEL;
			if(speed <= 0)		break;
		}
		else if(speed < fullSpeed) {		//accelerate to fullSpeed
			speed += MAX_ACCEL*TIME_STEP;
			if(speed > fullSpeed)	speed = fullSpeed;
		}

		//update nominal trajectory
		double dTheta_dt = curveByDist(dist)*speed;
		double oldYaw = nomYaw.back();
		double newYaw = oldYaw+dTheta_dt*TIME_STEP;
		nomYaw.push_back(newYaw);
		double newX = nomP[X].back()+speed*cos(oldYaw)*TIME_STEP;
		nomP[X].push_back(newX);
		double newY = nomP[X].back()+speed*sin(oldYaw)*TIME_STEP;
		nomP[Y].push_back(newY);

		dist += speed*TIME_STEP;

		ContactState oldContact = cs.back();
		switch(oldContact) {
		case SSL:
			if(segmentTime > SS_TIME) {	//right foot touchdown
				cs.push_back(DSc);
				segmentTime = 0.0;
				nextSwgFoot = LEFT;
				steps[RIGHT].back()->lo = time-SS_TIME;	//beginning of swing
				Step *s = new Step(newX+sin(newYaw)*STANCE_WIDTH/2.0, newY-cos(newYaw)*STANCE_WIDTH/2.0, newYaw, time, RIGHT);
				addStep(s);
			}
			else	cs.push_back(oldContact);
			break;
		case SSR:
			if(segmentTime > SS_TIME) {	//left foot touchdown
				cs.push_back(DSc);
				segmentTime = 0.0;
				nextSwgFoot = RIGHT;
				steps[LEFT].back()->lo = time-SS_TIME;	//beginning of swing
				Step *s = new Step(newX-sin(newYaw)*STANCE_WIDTH/2.0, newY+cos(newYaw)*STANCE_WIDTH/2.0, newYaw, time, LEFT);
				addStep(s);
			}
			else	cs.push_back(oldContact);
			break;
		case DSc:
			if((firstDS && segmentTime > DS0_TIME) || (!firstDS && segmentTime > DS_TIME)) {	//lift off
				firstDS = false;
				if(nextSwgFoot == LEFT)		cs.push_back(SSR);
				else						cs.push_back(SSL);
				segmentTime = 0.0;
			}
			else	cs.push_back(oldContact);
			break;
		default:
			printf("Bad oldContact\n");
			break;
		}
	}

	//walk an exact distance
	//add a final footstep at the end
	if(exact && segmentTime > 0.05) {
		int swgFoot = nextSwgFoot;

		if(cs.back() == DSc) {	//if in DS
			while((firstDS && segmentTime <= DS0_TIME) || (!firstDS && segmentTime <= DS_TIME)) {	//complete DS
				segmentTime += TIME_STEP;
				time += TIME_STEP;
				nomYaw.push_back(nomYaw.back());
				nomP[X].push_back(nomP[X].back());
				nomP[Y].push_back(nomP[Y].back());
				cs.push_back(DSc);
			}
			segmentTime = 0.0;
		}
		else {	//if already in SS
			swgFoot = 1-cs.back();
		}

		//complete SS
		while(segmentTime <= SS_TIME) {
			segmentTime += TIME_STEP;
			time += TIME_STEP;
			nomYaw.push_back(nomYaw.back());
			nomP[X].push_back(nomP[X].back());
			nomP[Y].push_back(nomP[Y].back());
			cs.push_back((ContactState)(1-swgFoot));
		}

		if(swgFoot == LEFT) {
			Step *s = new Step(nomP[X].back()-sin(nomYaw.back())*STANCE_WIDTH/2.0, nomP[X].back()+cos(nomYaw.back())*STANCE_WIDTH/2.0, nomYaw.back(), time, LEFT);
			addStep(s);
		}
		else {
			Step *s = new Step(nomP[X].back()+sin(nomYaw.back())*STANCE_WIDTH/2.0, nomP[X].back()-cos(nomYaw.back())*STANCE_WIDTH/2.0, nomYaw.back(), time, RIGHT);
			addStep(s);
		}

		//set up in case of match
		segmentTime = 0.0;
		time += TIME_STEP;
		nomYaw.push_back(nomYaw.back());
		nomP[X].push_back(nomP[X].back());
		nomP[Y].push_back(nomP[Y].back());
		cs.push_back(DSc);
	}

	//finish with matched footsteps
	if(match) {
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
	}
}
*/

void Plan::addStep(Step *step) {
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
			printf("startDDPind too large %d %d\n", startDDPind, com[X].size());
			exit(-1);
		}

		//run DDP in both x and y
		for(int i = 0; i < 2; i++) {
			double x0[2] = {(com[i])[startDDPind], (comd[i])[startDDPind]};
			ddp->getTrajs(x0, startDDPind, zmpVec[i], com[i], comd[i]);
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
	startDDPind = (t+0.1)/TIME_STEP;
	//TODO: implement
}

