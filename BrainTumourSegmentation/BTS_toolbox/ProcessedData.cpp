#include "ProcessedData.h"
#include <fstream>

bts::ProcessedData::ProcessedData()
{
}


bts::ProcessedData::~ProcessedData()
{
}

void bts::ProcessedData::evaluate(std::vector<bts::Slice> groundTruthSlices)
{
	// if already evaluated do not evaluate it again
	// if the number of GT slices do not fit the number of slices do not evaluate
	if (this->evaluatedSlices.size() > 0 || this->slices.size() != groundTruthSlices.size())
	{
		return;
	}
	// Create result file
	std::string resultPath = "data\\SegmentationResults\\" + this->patient->getPatientId() + ".csv"; //TODO make customable path
	std::ofstream resultFile(resultPath);
	resultFile << "Dice,Jaccard,TP,FP,TN,FN\n";
	long tTP = 0, tFP = 0, tFN = 0, tTN = 0;
	float tDice = 0, tJaccard = 0;
	for (int i = 0; i < this->slices.size(); i++)
	{
		EvaluatedSlice eSlice;
		cv::Mat gtImg = groundTruthSlices.at(i).getData();
		gtImg.convertTo(gtImg, CV_32F);
		cv::threshold(gtImg, gtImg, 0, 1, CV_THRESH_BINARY);
		cv::Mat img = slices.at(i).getData();
		img.convertTo(img, CV_32F); 
		eSlice.setNumber(slices.at(i).getNumber());

		cv::Mat and;
		cv::bitwise_and(img, gtImg, and);
		
		eSlice.TP = cv::countNonZero(and);
		eSlice.FP = cv::countNonZero(img) - eSlice.TP;
		eSlice.FN = cv::countNonZero(gtImg) - eSlice.TP;
		eSlice.TN = (gtImg.cols * gtImg.rows) - (eSlice.TP + eSlice.FP + eSlice.FN);

		// Jaccard
		eSlice.jaccard = eSlice.TP / float(eSlice.TP + eSlice.FP + eSlice.FN);

		// Dice
		int TP2 = 2 * eSlice.TP;
		eSlice.dice = TP2 / float(TP2 + eSlice.FP + eSlice.FN);

		// Visualisation
		cv::Mat rgbResult;
		rgbResult = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
		float* segmentedData = (float*)img.data;
		float* gtData = (float*)gtImg.data;
		for (int j = 0; j < img.rows; j++)
		{
			for (int k = 0; k < img.cols; k++)
			{
				if (gtData[img.cols*j + k] != 0)
				{
					if (segmentedData[img.cols*j + k] != 0)
					{
						rgbResult.data[img.cols*j * 3 + k * 3 + 1] = 255;
					}
					else
					{
						rgbResult.data[img.cols*j * 3 + k * 3 + 2] = 255;
					}
				}
				else if (segmentedData[img.cols*j + k] != 0)
				{
					rgbResult.data[img.cols*j * 3 + k * 3 + 0] = 255;
				}
			}
		}

		eSlice.setData(rgbResult);

		evaluatedSlices.push_back(eSlice);

		// Save results to file
		resultFile << std::to_string(eSlice.dice) + "," +
			std::to_string(eSlice.jaccard) + "," +
			std::to_string(eSlice.TP) + "," +
			std::to_string(eSlice.FP) + "," +
			std::to_string(eSlice.TN) + "," +
			std::to_string(eSlice.FN) + "\n";

		// Calculate total 
		tTP += eSlice.TP;
		tFP += eSlice.FP;
		tTN += eSlice.TN;
		tFN += eSlice.FN;
		tJaccard = tTP / float(tTP + tFP + tFN);
		tDice = (2 * tTP) / float((2 * tTP) + tFP + tFN);
		
	}
	// Save total
	resultFile << std::to_string(tDice) + "," +
		std::to_string(tJaccard) + "," +
		std::to_string(tTP) + "," +
		std::to_string(tFP) + "," +
		std::to_string(tTN) + "," +
		std::to_string(tFN) + "\n";
	resultFile.close();

	// Create TOTAL result file
	resultPath = "data\\SegmentationResults\\Totals.csv";
	std::ofstream fileTotal(resultPath, std::ios_base::app);
	resultFile << "Name,Dice,Jaccard,TP,FP,TN,FN\n";
	// Save total separetely
	fileTotal << this->patient->getPatientId() + "," +
		std::to_string(tDice) + "," +
		std::to_string(tJaccard) + "," +
		std::to_string(tTP) + "," +
		std::to_string(tFP) + "," +
		std::to_string(tTN) + "," +
		std::to_string(tFN) + "\n";
	fileTotal.close();
}