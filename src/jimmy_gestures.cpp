/*Ideally, this code would take input from send_gesture and convert the gesture values
we need from our numbers to desired servos and positions for the dynamixels. This information would be sent to
test_send_servo_cmd, which would then be interpreted by jimmy_servos 
We should be sending an array size 23 of every one of the joints to the subscribers.
*/

#include "Plan.h"
#include "Logger.h"
#include "IK.h"
#include "ControlUtils.h"
#include "Utils.h"

#include <ros/ros.h>
#include <ros/package.h>
#include <boost/thread.hpp>
#include <jimmy/jimmy_gesture.h>
#include <stdlib.h>
 
//#define SIMULATION

static const int8_t default_gain[TOTAL_JOINTS] = 
{
  32, 32, 60, 32, 60, 32,  
  32, 32, 60, 32, 60, 32,  
  32, 32, 32, 32,
  32, 32, 32, 32,
  32, 32, 32
};

static const int8_t walk_gain[TOTAL_JOINTS] = 
{
  40, 60, 120, 60, 60, 120,
  40, 60, 120, 60, 60, 120,
  32, 32, 32, 32,
  32, 32, 32, 32,
  32, 32, 32
};

///////////////////////////////////////////////////
// ros stuff
boost::mutex r_Lock;
static double r_vFwd = 0;
static double r_vLeft = 0;
static double r_dTheta = 0;
static int r_mode = 0;
static double r_neckEAd[3] = {0};

ros::Publisher pub_feedback;

void cleanCommand()
{
  printf("starting clean command\n");
  //boost::mutex::scoped_lock lock(r_Lock);
  printf("boost clear\n");
  r_vFwd = 0;
  r_vLeft = 0;
  r_dTheta = 0;
  printf("roll yaw tilt clear\n");
  r_mode = 0;
  r_neckEAd[0] = 0; 
  r_neckEAd[1] = 0; 
  r_neckEAd[2] = 0; 
  printf("neck EAs clear\n");
}



///////////////////////////////////////////////////
 
Plan plan;
Logger logger;
IKcmd IK_d;
IKcon IK;
#ifndef SIMULATION
ControlUtils utils;
#endif

const int MAX_POSES = 100;
const int MAX_GESTURES = 100;
const int MAX_POSES_PER_GESTURE = 100;
const int N_VALS_PER_POSE = 17;

static int N_POSES;

static int prevStance = 2;
static bool prevLimp[2] = {false, false};

static double poses[MAX_POSES][N_VALS_PER_POSE];
static int gestureSeq[MAX_GESTURES][MAX_POSES_PER_GESTURE];
static double gestureTime[MAX_GESTURES][MAX_POSES_PER_GESTURE];
static int gestureNpose[MAX_GESTURES];

static double neckEAs[3];
static double neckAngs[3];
static double theta_d[N_J+3];

static double joints_actual[TOTAL_JOINTS];

const static double neckLims[2][3] = {
	{-1.4, -0.7, -0.7},
	{1.4, 0.7, 0.7}
};

const static double neckEAlims[2][3] = {
	{-1.4, -0.6, -0.6},
	{1.4, 0.6, 0.6}
};

const static double poseBodyLims[2][6] = {
	{-0.1, -0.1, 0.35, -0.3, -0.3, -0.3},
	{0.1, 0.1, 0.45, 0.3, 0.3, 0.3}
};

enum ConMode {
	IDLE=0,
	PRE_WALK,	//wait until we have enough steps queued up
	WALK,
	GESTURE,
	STAND_PREP
};

//static double wallClockStart, wallClockLast;
static double wallClockDT, wallClockT;

static bool isIdle;
static ConMode mode;

static double curTime;		//time since start of controller
static double modeTime;	//time since start of current mode
static double modeT0;		//time this mode started;
static double modeDur;		//intended duration of current mode

static double vForward, vLeft, dTheta;

int getCommand() {
  boost::mutex::scoped_lock lock(r_Lock);
  printf("currently getting command\n");
  return r_mode;
}

TrajEW spJoints[23];
TrajEW gestureArms[8];
TrajEW gestureNeck[3];
TrajEW gestureCoM[3];
TrajEW gestureRootEA[3];

void initGesture(int gesture) {
	//gesture-=2;
	printf("Starting gesture %d\n", gesture);
	//initialize with current state
	for(int i = 0; i < 8; i++) {
		gestureArms[i].clear();
		gestureArms[i].addKnot(0, IK_d.armJoints[i], 0, 0);
	}
	printf("init arms clear\n");
	for(int i = 0; i < 3; i++) {
		gestureNeck[i].clear();
		gestureNeck[i].addKnot(0, neckEAs[i], 0, 0);
	}
	printf("init neck clear\n");
	for(int i = 0; i < 3; i++) {
		gestureCoM[i].clear();
		gestureCoM[i].addKnot(0, IK_d.com[i]-(IK_d.foot[LEFT][i]+IK_d.foot[RIGHT][i])/2.0, 0, 0);
	}
	double rootEA[3];
	printf("init feet clear\n");
	quat2EA(IK_d.rootQ, rootEA);
	for(int i = 0; i < 3; i++) {
		gestureRootEA[i].clear();
		gestureRootEA[i].addKnot(0, rootEA[i], 0, 0);
	}

	Eigen::Quaterniond avgFootQ = mySlerp(IK_d.footQ[LEFT], IK_d.footQ[RIGHT], 0.5);
	double footYaw = getYaw(avgFootQ);
	if(footYaw - rootEA[2] > M_PI)		footYaw -= M_PI*2.0;
	if(footYaw - rootEA[2] < -M_PI)		footYaw += M_PI*2.0;

	double totTime = 0;
	//add the poses to the trajectories
	printf("adding poses to trajectories\n");
	for(int p = 0; p < gestureNpose[gesture]; p++) {
		totTime += gestureTime[gesture][p];
		int pose = gestureSeq[gesture][p];
		for(int i = 0; i < 8; i++) 	gestureArms[i].addKnot(totTime, poses[pose][i], 0, 0);
		for(int i = 0; i < 3; i++)	gestureNeck[i].addKnot(totTime, poses[pose][i+8], 0, 0);
		for(int i = 0; i < 3; i++)	gestureCoM[i].addKnot(totTime, poses[pose][i+11], 0, 0);
		for(int i = 0; i < 2; i++)	gestureRootEA[i].addKnot(totTime, poses[pose][i+14], 0, 0);
						gestureRootEA[2].addKnot(totTime, footYaw+poses[pose][16], 0, 0);
					printf("added 1 pose\n");
	}
	printf("init gesture clear\n");
	//when the gesture ends and returns to IDLE
	modeDur = totTime;
}

//static const double armsOut[8] = {-0.33, -0.2, 1.03, 0, -0.33, 0.2, 1.03, 0};

void loadPoses() {
	std::string name = ros::package::getPath("jimmy") + "/conf/poses.cf";
	std::ifstream in(name.c_str());

	int pose = 0;
	int i = 0;
	while (true) {
		in >> poses[pose][i];
		//std::cout << i << " " << poses[pose][i] << std::endl;
		i++;
		if(i >= N_VALS_PER_POSE) {
			i = 0;
			pose++;
		}


		if (in.eof()) {
			if(i != 0) {
				printf("Ran out of data mid pose, in pose %d\n", pose);
				exit(-1);
			}	
			printf("Loaded %d poses\n",pose);
			break;
		}
	}
	N_POSES = pose;

	//assemble pose limits from other limits
	double poseLimits[2][N_VALS_PER_POSE];
	for(int m = 0; m < 2; m++) {
		for(int i = 0; i < 8; i++)	poseLimits[m][i] = RobotState::jointLimits[m][i+12];
		for(int i = 0; i < 3; i++)	poseLimits[m][i+8] = neckEAlims[m][i];
		for(int i = 0; i < 6; i++)	poseLimits[m][i+11] = poseBodyLims[m][i];
	}

	for(int p = 0; p < pose; p++) {
		for(int i = 0; i < N_VALS_PER_POSE; i++) {
			if(poses[p][i] < poseLimits[0][i]) {
				//printf("Limiting pose %d value %d from %g to %g\n", p, i, poses[p][i], poseLimits[0][i]);
				poses[p][i] = poseLimits[0][i];

			}
			if(poses[p][i] > poseLimits[1][i]) {
				//printf("Limiting pose %d value %d from %g to %g\n", p, i, poses[p][i], poseLimits[1][i]);
				poses[p][i] = poseLimits[1][i];
			}
		}
	}
}

void loadGestures() {
	std::string name = ros::package::getPath("jimmy") + "/conf/gestures.cf";
	std::ifstream in(name.c_str());

	int gesture = 0;
	while (true) {
		in >> gestureNpose[gesture];
		if (in.eof()) 	break;
		for(int i = 0; i < gestureNpose[gesture]; i++) {
			in >> gestureSeq[gesture][i];
			if(gestureSeq[gesture][i] < 0) {
				printf("We only use positive pose ID's: %d\n",gestureSeq[gesture][i]);
				exit(-1);
			}
			if(gestureSeq[gesture][i] >= N_POSES) {
				printf("We only have %d poses stored: %d\n",N_POSES, gestureSeq[gesture][i]);
				exit(-1);
			}
			in >> gestureTime[gesture][i];
			if(gestureTime[gesture][i] <= 0) {
				printf("Require a positive time in gesture %d, pose %d: %g\n", gesture, i, gestureTime[gesture][i]);
				exit(-1);
			}
		}
		if (in.eof()) {
			printf("Ran out of data mid gesture, in gesture: %d \n", gesture);
			exit(-1);
		}
		gesture++;
	}
	printf("Loaded %d gestures\n",gesture);
}

void initStandPrep() {
#ifndef SIMULATION
	utils.getJoints(joints_actual);
#endif
	for(int i = 0; i < 23; i++)		
    spJoints[i].freshMove(joints_actual[i], standPrepPose[i], 5.0);
	modeDur = 5.0;
}

void init() {
	printf("Start init\n");
	loadPoses();
	loadGestures();
	isIdle = false;
	curTime = 0.0;
	modeTime = 0.0;
	modeT0 = 0.0;
	modeDur = 0.0;
	for(int i = 0; i < 3; i++)		neckEAs[i] = 0.0;

  int8_t thermal_max[TOTAL_JOINTS] = {0};
  for (int i = 0; i < TOTAL_JOINTS; i++)
    thermal_max[i] = 60;
#ifndef SIMULATION
  assert(utils.setThermalMax(thermal_max));
  //assert(utils.getThermalMax(thermal_max));
#endif
  for (int i = 0; i < TOTAL_JOINTS; i++)
    printf("max thermal %10s %d\n", RobotState::jointNames[i].c_str(), thermal_max[i]);
  
#ifndef SIMULATION
  assert(utils.setPGains(default_gain));
#endif

	std::string name = ros::package::getPath("jimmy") + "/conf/plan.cf";
	plan = Plan(name.c_str());
	name = ros::package::getPath("jimmy") + "/conf/IK.cf";
	IK.readParams(name.c_str());
	logger.init(plan.TIME_STEP);

  char buf[100];
  for (int i = 0; i < TOTAL_JOINTS; i++) {
    sprintf(buf, "RS.joint[%s]", RobotState::jointNames[i].c_str());
    logger.add_datapoint(buf,"rad",joints_actual+i); 
  }
  
  logger.add_datapoint("realT","s",&wallClockT);
	logger.add_datapoint("realDT","s",&wallClockDT);
	logger.add_datapoint("curTime","s",&curTime);
	logger.add_datapoint("modeTime","s",&modeTime);
	logger.add_datapoint("mode","-",(int*)(&mode));
	logger.add_datapoint("CMD.vForward","m/s",&vForward);
	logger.add_datapoint("CMD.vLeft","m/s",&vLeft);
	logger.add_datapoint("CMD.dTheta","m/s",&dTheta);
	logger.add_datapoint("stance","-",&prevStance);
	logger.add_datapoint("limpA[L]","-",&(prevLimp[LEFT]));
	logger.add_datapoint("limpA[R]","-",&(prevLimp[RIGHT]));
	IK_d.addToLog(logger);
	IK.addToLog(logger);
	plan.addToLog(logger);

  for (int i = 0; i < TOTAL_JOINTS; i++) {
    sprintf(buf, "CMD.%s", RobotState::jointNames[i].c_str());
    logger.add_datapoint(buf,"rad",theta_d+i); 
  }
	IK_d.setToRS(IK.ikrs);
	IK_d.setVel0();
	mode = IDLE;
	//initStandPrep();
}

void stateMachine() {
	int command; //, random_gesture;
	printf("INSTATEMACHINE\n");
	printf("MODE IS %d\n",mode);
	switch(mode) {
	case IDLE:
		printf("IDLEMODE");
/*		srand (time(NULL)); //seeds random number
		random_gesture = rand() % 7 + 2;*/
		command = getCommand();
/*		modeT0 = curTime;
		mode = GESTURE;
		initGesture(random_gesture);
		printf("IDLE to GESTURE\n");*/

		//printf("IDLE GESTURE\n");

		//don't want to make this unreadable with a switch inside a switch
		if(command != 0) {	//each number corresponds to a gesture
			modeT0 = curTime;
			mode = GESTURE;
			initGesture(command);
      		cleanCommand();
			printf("IDLE to GESTURE\n");
		}

		break;


	case PRE_WALK:
		if(modeTime > plan.PRE_PLAN_TIME) {
			modeT0 = curTime;
			mode = WALK;
			printf("PRE_WALK to WALK\n");
#ifndef SIMULATION
      assert(utils.setPGains(walk_gain));
      assert(utils.setStanceGain(2));
      printf("setting walking gain\n");
#endif
		}
		break;
	case WALK:
		if(getCommand() == 0) {
			plan.stopHere();
		}
		if(plan.isDone(modeTime) || modeTime > 36) {
			modeT0 = curTime;
			mode = IDLE;
			printf("WALK to IDLE\n");
      assert(utils.setPGains(default_gain));
      printf("setting default gain\n");
      cleanCommand();
		}
		break;
	case GESTURE:
		if(modeTime > modeDur) {
			modeT0 = curTime;
			mode = IDLE;
			printf("GESTURE to IDLE\n");
		}
		break;
	case STAND_PREP:
		if(modeTime > modeDur) {
			modeT0 = curTime;
			mode = IDLE;
			printf("STAND_PREP to IDLE\n");
		}
		break;
	default:
		printf("Bad mode in stateMachine\n");
		exit(-1);
		break;
	}
}


#if 0
void stateMachine() {
	int command = r_mode;
	printf("command recieved\n");
	std::cout << "Registered command  " << command << std::endl;
	if (command != 0) {
		mode = GESTURE;
		initGesture(command);
	} else {
		mode = IDLE;
	}
//	printf("statemachine init gesture clear\n");
//	cleanCommand();
	printf("clean command clear\n");
	if(modeTime > modeDur) {
		modeT0 = curTime;
		mode = IDLE;
	}
}
#endif
//double nomPose[3] = {0.0, 0.0, 0.42};

void gestureCon() {
	printf("starting gestureCon\n");
	for(int i = 0; i < 8; i++) 	{
		IK_d.armJoints[i] = gestureArms[i].readPos(modeTime);
		std::cout << "read position arms  "<< gestureArms[i].readPos(modeTime) << std::endl;
		std::cout << "IK arms  "<< IK_d.armJoints[i] << std::endl;
	}
	printf("finish IK for arms\n");
	
	for(int i = 0; i < 3; i++){
		neckAngs[i] = gestureNeck[i].readPos(modeTime);
		std::cout << "read position neck  "<< gestureNeck[i].readPos(modeTime) << std::endl;
		std::cout << "neckAngs  "<< neckAngs[i] << std::endl;
	}
	printf("finish IK for neck\n");
	for(int i = 0; i < 3; i++) {
		gestureCoM[i].read(modeTime, &(IK_d.com[i]), &(IK_d.comd[i]), NULL);
		IK_d.com[i] += (IK_d.foot[LEFT][i] + IK_d.foot[RIGHT][i])/2.0;
		std::cout << "foot angs  "<< IK_d.com[i] << std::endl;
	}
	double rootEA[3];
	printf("finish IK for feet\n");
	for(int i = 0; i < 3; i++){	rootEA[i] = gestureRootEA[i].readPos(modeTime);
	IK_d.rootQ = EA2quat(rootEA);
	printf("finished gestureCon\n");

}}


void standPrepCon() {
	for(int i = 0; i < 23; i++)		theta_d[i] = spJoints[i].readPos(modeTime);
#ifndef SIMULATION
	utils.setJoints(theta_d);
#endif

}


void controlLoop() {

	//handle mode switching
	stateMachine();

	modeTime = curTime-modeT0;

	//whipe out record values
	vForward = vLeft = dTheta = 0.0;
	plan.clearForRecord();
	
#ifndef SIMULATION
  utils.getLegJointsCircular(joints_actual);
#endif
  gestureCon();
	for(int i = 0; i < 3; i++){	theta_d[N_J+i] = neckAngs[i];
		neckEAs[0] = (-neckAngs[2] - neckAngs[1])/2.0;
		neckEAs[1] = ( neckAngs[2] - neckAngs[1])/2.0;
		neckEAs[2] = neckAngs[0];
	}
#ifndef SIMULATION
		utils.setJoints(theta_d);
#endif
	}

void jimmyGestureCallback(const jimmy::jimmy_gesture &msg)
{
  boost::mutex::scoped_lock lock(r_Lock);
  
  r_mode = msg.cmd;
  printf("%d\n",r_mode);
}

#if 0
void jimmyGestureCallback(const jimmy::jimmy_gesture &msg)
{
  boost::mutex::scoped_lock lock(r_Lock);
  int newMode = msg.cmd;
  std::cout << "received command  " << newMode << std::endl;
  r_mode = newMode;
  std::cout << "r_mode is  " << r_mode << std::endl;

  stateMachine();
  printf("stateMachine clear\n");
  modeTime = curTime-modeT0;

	//whipe out record values
	vForward = vLeft = dTheta = 0.0;
	plan.clearForRecord();
	printf("clear for record clear\n");
#ifndef SIMULATION
  utils.getLegJointsCircular(joints_actual);
  printf("get joints circular clear\n");
#endif
  
  gestureCon();
  	for (int i = 0; i < 20; ++i)
	{
		std::cout<< "gestureCon joint value  " << theta_d[i] << std::endl;
	}
	for(int i = 0; i < 3; i++){	theta_d[N_J+i] = neckAngs[i];
		neckEAs[0] = (-neckAngs[2] - neckAngs[1])/2.0;
		neckEAs[1] = ( neckAngs[2] - neckAngs[1])/2.0;
		neckEAs[2] = neckAngs[0];
	}
#ifndef SIMULATION
		utils.setJoints(theta_d);
	for (int i = 0; i < 20; ++i)
	{
		std::cout<< "setjoint value  " << theta_d[i] << std::endl;
	}
		printf("joints set\n");
#endif
	}
#endif

int sleep_t = 1e4;
double t_pre_sleep = 0;
double t_aft_sleep = 0;
double t_real_sleep = 0;


void spin_wait(double dt)
{
  double t0 = get_time();
  int spin = 1;
  int ctr = 0;
  while(spin) {
    if (get_time() - t0 < dt) {
      ctr++;
    }
    else
      spin = 0;
  }

  return;
}

int main( int argc, char **argv ) 
{
	init();
  ////////////////////////////////////////////////////
  // ros stuff
  ros::init(argc, argv, "jimmy_gestures", ros::init_options::NoSigintHandler);
  ros::NodeHandle rosnode = ros::NodeHandle();
  printf("Ready to listen for commands!\n");
 
  	ros::Subscriber subcommand = rosnode.subscribe("jimmy_send_gesture", 10, jimmyGestureCallback);
  	//////////////////////////////////////////////////// 
	
	
   	for (;;) {
   	 controlLoop();
   	//printf("control loop is spinning\n");
	//printf("Spinning!\n");
     ros::spinOnce();
     sleep(1);
   }
	
	return 0;
}