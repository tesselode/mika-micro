#include "Filter.h"

// state variable filter
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=23

double Filter::Process(double input, double cutoff)
{
	double f = cutoff / sampleRate * 2;
	low += f * band;
	double high = scale * input - low - q * band;
	band += f * high;
	return low;
}
