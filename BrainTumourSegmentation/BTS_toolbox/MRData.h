#pragma once
#include "Slice.h"
#include "segfunc.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace bts
{
	class Patient;

	class MRData
	{
	
	public:
		MRData();
		~MRData();
	private:
		Patient* patient; // reference to the patient
		int sliceCount; // number of slices
		int intensityMaxs[modalityCount];
		int globalIntensityMax;
		std::vector<Slice> slices[modalityCount];

	public:
		void calculateMaxIntensities();

		Patient* getPatient() const
		{
			return patient;
		}

		void setPatient(Patient* patient)
		{
			this->patient = patient;
		}

		int getSliceCount() const
		{
			return sliceCount;
		}

		void setSliceCount(int slice_count)
		{
			sliceCount = slice_count;
		}

		int getIntensityMax(int modality)
		{
			return intensityMaxs[modality];
		}

		int getIntensityMax(std::string modality)
		{
			return intensityMaxs[modalityMap[modality]];
		}

		void setIntensityMax(int intensity, int modality)
		{
			intensityMaxs[modality] = intensity;
		}

		void setIntensityMax(int intensity, std::string modality)
		{
			intensityMaxs[modalityMap[modality]] = intensity;
		}

		int getGlobalIntensityMax()
		{
			return globalIntensityMax;
		}

		void setGlobalIntensityMax(int intensity)
		{
			globalIntensityMax = intensity;
		}

		std::vector<Slice> getSlices(int modality)
		{
			return slices[modality];
		}
		
		void setSlices(std::vector<Slice> slices, int modality)
		{
			this->slices[modality] = std::move(slices);
		}

	};
}
