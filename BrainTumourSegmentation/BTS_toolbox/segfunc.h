#pragma once
#include <map>
#include <vector>
#include "Slice.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
// if i include proccesseddata.h i get undeclared identifier modalityCount because of crossincluding can be fix with forward declaraction
#include "D:/FIIT/GIT/Source/Repos/gSLICr/gSLICr-master/gSLICr_Lib/gSLICr.h"
#include <map>
#include <set>

namespace bts
{
	class Patient;

	static std::map<std::string, int> modalityMap = 
	{ { "T1", 0 },
	{ "T1c", 1 },
	{ "T2", 2 },
	{ "Flair", 3 },
	{ "GT", 4 } };

	// Global configuration variables
	const int modalityCount = 5;
	static bool trainingPurpose = true;
	static bool evaluateSPX = false;
	static float spxCompactness = 0.05;
	static int spxSideSize = 16;
	static int iterations = 5;
	
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
	cv::Point2i getCentroid(std::vector<cv::Point2i> superpixel);

	// Growing region segmentation
	std::vector<bts::Slice> growingRegion(std::vector<bts::Slice> mask, cv::Point3i seed, int connectivity);

	// Fix seed when does not hit
	cv::Point3i findNearestPositive(std::vector<bts::Slice> mask, cv::Point3i centroid);
	
	void doComplexSegmentation(Patient* patient);

	// TODO prerobit
	void convertMhaToCNTKtxt(std::vector<bts::Patient> &patients, std::vector<bool> sequences);

	// Superpixelisation
	//std::vector<Slice> calculateSuperpixels(bts::Patient* patient, int spxSize, float compactness, int iterations, bool enforceConnectivity, std::vector<bool> features);
	std::vector<Slice> calculateSuperpixels(bts::Patient* patient, std::map<std::string, float> *configuration);
	void gSLICrLoadImage(const cv::Mat& inimg, gSLICr::UChar4Image* outimg);
	void gSLICrLoadImage(const gSLICr::UChar4Image* inimg, cv::Mat& outimg);
	void gSLICrLoadImage(const gSLICr::IntImage* inimg, cv::Mat& outimg);
	void gSLICrLoadImage(const cv::Mat& one, const cv::Mat& two, const cv::Mat& three, gSLICr::UChar4Image* outimg);
	std::vector<std::vector<cv::Point2i>> getSuperpixelsFromMask(const int* mask, std::vector<std::set<int>>& neighbourhood, const int noSPX, int cols, int rows);
	std::vector<std::vector<std::vector<int>>> getAllPixelIntensitiesOfSuperpixels(const std::vector<bts::Slice>& fSlices, const std::vector<std::vector<cv::Point2i>>& spxLoc);

	// Feature extraction
	void calculateFeaturesOfSuperpixels(const Slice& slice, const cv::Mat& gt, const int* mask, int noSPX, float nf, cv::Point3i centroid, std::vector<bool> calculateFeatures, std::vector<std::vector<float>>& tumorousFeatures, std::vector<std::vector<float>>& nonTumorousFeatures);
	void calculateFeaturesOfSuperpixels(const std::vector<bts::Slice>& fSlices, const cv::Mat& gt, const int* mask, int noSPX, const std::vector<float>& fNFs, cv::Point3i centroid, std::vector<bool> calculateFeatures, std::vector<std::vector<float>>& tumorousFeatures, std::vector<std::vector<float>>& nonTumorousFeatures);
	void saveSuperpixelsFeaturesToFile(std::vector<std::vector<float>>& tumorousFeatures, std::vector<std::vector<float>>& nonTumorousFeatures, std::string fileName);
	bool isAllIntesitiesLess(int threshold, std::vector<int> superpixel);
	bool isAllIntesitiesLess(int threshold, std::vector<int> superpixel, float percentage);
	int isTumour(std::vector<cv::Point2i> superpixel, const cv::Mat& gt, bool calculateQuality);
	float entropy(int *histogram, int bins, int numel);

}


