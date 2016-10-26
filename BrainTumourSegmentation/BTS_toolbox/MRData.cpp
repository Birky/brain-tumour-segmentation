#include "MRData.h"
#include "segfunc.h"


bts::MRData::MRData()
{
}


bts::MRData::~MRData()
{
}

void bts::MRData::calculateMaxIntensities()
{
	if (slices->size() != 0)
	{
		globalIntensityMax = 0;
		for (int i = 0; i < modalityCount; i++)
		{
			intensityMaxs[i] = 0;
			for each(Slice slice in slices[i])
			{
				double sliceMin, sliceMax;
				cv::minMaxLoc(slice.getData(), &sliceMin, &sliceMax);
				intensityMaxs[i] = sliceMax > intensityMaxs[i] ? sliceMax : intensityMaxs[i];
			}
			globalIntensityMax = intensityMaxs[i] > globalIntensityMax ? intensityMaxs[i] : globalIntensityMax;
		}
		
	}
}
