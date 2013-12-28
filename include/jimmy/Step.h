#ifndef STEP_DEFINED
#define STEP_DEFINED

#include <limits>

class Step {
public:
	double x, y, yaw;
	double td, lo;
	int side;

	Step(double X, double Y, double YAW, double TD, int SIDE) {
		x = X;
		y = Y;
		yaw = YAW;
		td = TD;
		lo = std::numeric_limits<double>::infinity();
		side = SIDE;
	}
};

#endif
