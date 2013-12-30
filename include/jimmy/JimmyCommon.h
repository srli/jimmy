#ifndef JCOMMON_DEFINED
#define JCOMMON_DEFINED

#define GRAVITY 9.8

const int N_JOINTS = 20;        // sdfast
const int N_J = N_JOINTS;
const int N_U = N_J+6;
const int N_Q = N_U+1;

const int TOTAL_JOINTS = 23;    // real

const double standPrepPose[TOTAL_JOINTS] = {
	0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	0, -0.6, 0, 0,
	0,  0.6, 0, 0,
  0, 0, 0
};

enum Joint{
	L_HZ=0,
	L_HFE,
	L_HAA,
	L_KFE,
	L_AFE,
	L_AAA,
	R_HZ,
	R_HFE,
	R_HAA,
	R_KFE,
	R_AFE,
	R_AAA,
	L_SFE,
	L_SAA,
	L_SR,
	L_ELB,
	R_SFE,
	R_SAA,
	R_SR,
	R_ELB,        // sdfast model ends here
  NECK_YAW,
  NECK_1,       // differential 
  NECK_2
};


enum Side {
	LEFT=0,
	RIGHT,
	LR
};

enum Dim {
	X=0,
	Y,
	Z,
	XYZ
};


enum Type {
	SS=0,
	DS
};

enum Contact {
	UP=0,
	DOWN
};


enum ContactState {
	SSL=0,
	SSR,
	DSc
};


#endif
