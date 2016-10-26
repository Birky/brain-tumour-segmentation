#pragma once
#include <string>
#include "MRData.h"
#include <vector>
#include "ProcessedData.h"

namespace bts
{
	class ProcessedData;
	class MRData;

	class Patient
	{
	public:
		Patient();
		~Patient();
	private:
		std::string patientID;
		std::string dataFolder;
		MRData* orginalData;
		std::vector<ProcessedData> processedData;

	public:
		std::string getPatientId() const
		{
			return patientID;
		}

		void setPatientId(std::string patient_id)
		{
			patientID = std::move(patient_id);
		}

		std::string getDataFolder() const
		{
			return dataFolder;
		}

		void setDataFolder(std::string data_folder)
		{
			dataFolder = std::move(data_folder);
		}

		MRData* getOrginalData() const
		{
			return orginalData;
		}

		void setOrginalData(MRData* orginal_data)
		{
			orginalData = orginal_data;
		}

		std::vector<ProcessedData> getProcessedData() const
		{
			return processedData;
		}

		void setProcessedData(std::vector<ProcessedData> processed_datas)
		{
			processedData = std::move(processed_datas);
		}

		ProcessedData* getProcessedData(std::string title);

		void addProcessedData(ProcessedData &pd);

	};
}

