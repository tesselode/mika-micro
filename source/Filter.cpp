#include "Filter.h"

// state variable filter
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=23

double Filter::Process(double input, double cutoff)
{
	double f = cutoff / sampleRate * 2;
	if (f > 1) f = 1;
	if (f < 0) f = 0;
	low += f * band;
	double high = scale * input - low - q * band;
	band += f * high;
	return low;
}
