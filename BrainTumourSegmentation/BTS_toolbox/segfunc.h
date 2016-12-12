#pragma once
#include <map>
#include <vector>
#include "Slice.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
// if i include proccesseddata.h i get undeclared identifier modalityCount because of crossincluding can be fix with forward declaraction

namespace bts
{
	class Patient;

	static std::map<std::string, int> modalityMap = 
	{ { "T1", 0 },
	{ "T1c", 1 },
	{ "T2", 2 },
	{ "Flair", 3 },
	{ "GT", 4 } };

	const int modalityCount = 5;
	
	bool isProcessedData(std::string name);

	int getPatientByName(std::vector<Patient> patients, std::string name);

	// Classic thresholding
	std::vector<Slice> doThreshold(std::vector<Slice> slices, float thresholdValue, float normalizationFactor); // Normalization factor is needed to normalize the 16U images to 32F 0 .. 1
	
	// Optimal thresholding 
	std::vector<Slice> doOptimalThreshold(std::vector<Slice> slices, int angle, float distance, int fromTop, float nf, bool minAvg);
	float findOptimalThresholdValue(cv::Mat img, int angle, float distance, int fromTop, float nf);
	std::vector<float> findOptimalThresholdValues(std::vector<Slice> slices, int angle, float distance, int fromTop, float nf);
	cv::Mat createHistogram(cv::Mat img, int bins);
	cv::Mat averageHistogram(cv::Mat hist, int distance);
	void showHistogram(cv::Mat hist, std::string winTitle);

	// Image(s) arithmetic operations
	std::vector<Slice> substruct(std::vector<Slice> slicesA, std::vector<Slice> slicesB, float nfA, float nfB);
	std::vector<Slice> logicalAND(std::vector<Slice> slicesA, std::vector<Slice> slicesB);

	// Convertions
	float *** convertSlicesTo3DArray(std::vector<Slice> slices);

	// Calculate centroid of given data
	cv::Point3i getCentroid(std::vector<Slice> slices);

	// Growing region segmentation
	std::vector<bts::Slice> growingRegion(std::vector<bts::Slice> mask, cv::Point3i seed, int connectivity);
	
	void doComplexSegmentation(Patient* patient);
}


