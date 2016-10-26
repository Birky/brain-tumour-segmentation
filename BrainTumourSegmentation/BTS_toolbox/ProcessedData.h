#pragma once
#include "Patient.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace bts
{
	class Slice;
	class Patient;

	class ProcessedData
	{
	public:
		ProcessedData();
		~ProcessedData();
	private:
		Patient* patient;
		std::string title;
		int sliceCount; // TODO asi zbytoèné staèí nám slices.size()
		int modality; // modality index
		std::vector<Slice> slices;

		// TODO vytvor funkciu pre evaluaciu
		std::vector<EvaluatedSlice> evaluatedSlices;


	public:

		void evaluate(std::vector<bts::Slice> groundTruthSlices);

		std::string getTitle()
		{
			return title;
		}

		void setTitle(std::string title)
		{
			this->title = title;
		}

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

		int getModality() const
		{
			return modality;
		}

		void setModality(int modality)
		{
			this->modality = modality;
		}

		std::vector<Slice> getSlices() const
		{
			return slices;
		}

		void setSlices(std::vector<Slice> slices)
		{
			this->slices = std::move(slices);
		}

		std::vector<EvaluatedSlice> getEvaluatedSlices() const
		{
			return evaluatedSlices;
		}

		void setSlices(std::vector<EvaluatedSlice> slices)
		{
			this->evaluatedSlices = std::move(slices);
		}
	};
}

