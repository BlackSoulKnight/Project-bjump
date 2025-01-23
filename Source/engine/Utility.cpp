#include "Utility.h"

double normalize(double min, double max, double val) {
	if (val < min)
		return 0;
	if (val > max)
		return 1;

}