#define _USE_MATH_DEFINES
#include "segfunc.h"
#include "Patient.h"
#include <numeric>
#include <algorithm> 
#include <fstream>


namespace bts
{
	// This function was created to produce input for convolutional neural networks, but we did not use it
	void convertMhaToCNTKtxt(std::vector<bts::Patient> &patients, std::vector<bool> sequences) //TODO remake these sequences
	{
		// Now only FLAIR, and GT for label
		std::ofstream outfile;
		outfile.open("training_flair_slices.txt", std::ios_base::app);

		for each (Patient pat in patients)
		{
			std::vector<bts::Slice> flair = pat.getOrginalData()->getSlices(modalityMap["Flair"]);
			std::vector<bts::Slice> gt = pat.getOrginalData()->getSlices(modalityMap["GT"]);


			for (int k = 0; k < gt.size(); k++)
			{
				outfile << "|labels ";
				cv::Mat img = gt.at(k).getData();
				//unsigned short *slice = (unsigned short*)img.data;
				for (int j = 0; j < img.rows; j++){
					for (int i = 0; i < img.cols; i++){
						unsigned short value = img.at<unsigned short>(j, i);
						//unsigned short value = slice[img.step * j + i];
						if (value != 0)
							outfile << 1 << " ";
						else
							outfile << 0 << " ";
					}
				}

				outfile << "|features ";
				img = flair.at(k).getData();
				for (int j = 0; j < img.rows; j++){
					for (int i = 0; i < img.cols; i++){
						//unsigned short value = slice[img.step * j + i];
						unsigned short value = img.at<unsigned short>(j, i);
						outfile << value << " ";
					}
				}
				outfile << "\n";
			}

			/*	outfile << "|features ";
				for (int k = 0; k < flair.size(); k++)
				{
				cv::Mat img = flair.at(k).getData();
				//unsigned short *slice = (unsigned short*)img.data;
				for (int j = 0; j < img.rows; j++){
				for (int i = 0; i < img.cols; i++){
				//unsigned short value = slice[img.step * j + i];
				unsigned short value = img.at<unsigned short>(j, i);
				outfile << value << " ";
				}
				}
				}
				outfile << "\n";*/
		}
	}

	std::vector<Slice> logicalAND(std::vector<Slice> slicesA, std::vector<Slice> slicesB)
	{
		for (int i = 0; i < slicesA.size(); i++)
		{
			cv::Mat imgA = slicesA.at(i).getData();
			cv::Mat imgB = slicesB.at(i).getData();
			cv::threshold(imgA, imgA, 0, 1, CV_THRESH_TOZERO);
			cv::threshold(imgB, imgB, 0, 1, CV_THRESH_TOZERO);
			cv::Mat result;
			cv::bitwise_and(imgA, imgB, result);
			slicesA.at(i).setData(result);
		}
		return slicesA;
	}

	int countNonZero(std::vector<bts::Slice> volume)
	{
		int sum = 0;
		for (int z = 0; z < volume.size(); z++)
		{
			sum += cv::countNonZero(volume.at(z).getData());
		}
		return sum;
	}

	std::vector<bts::Slice> getBiggestConnectedRegion(std::vector<bts::Slice> volume)
	{
		int originCount = countNonZero(volume);
		int currentCount = 0;
		int biggestCount = 0;

		std::vector<bts::Slice> biggestRegion;
		std::vector<bts::Slice> currentRegion;
		// Loop the volume
		for (int z = 0; z < volume.size(); z++)
		{
			for (int y = 0; y < volume.at(z).getData().rows; y++)
			{
				for (int x = 0; x < volume.at(z).getData().cols; x++)
				{
					if (volume.at(z).getData().at<float>(y, x) == 1.0)
					{
						currentRegion = growingRegion(volume, cv::Point3i(x, y, z), 0);
						currentCount = countNonZero(currentRegion);

						// If it is bigger or equal to the half we found the biggest region
						if (currentCount >= originCount)
						{
							return currentRegion;
						}
						else if (currentCount > biggestCount)
						{
							biggestCount = currentCount;
							biggestRegion = currentRegion;
						}
						// Substruct the currentRegion from the volume to do not search the same region again
						volume = substruct(volume, currentRegion, 1, 1);
					}
				}
			}
		}
		return biggestRegion;
	}

	void doComplexSegmentation(Patient* patient)
	{
		std::vector<bts::Slice> processedSlices;

		// get slices of Flair modality
		std::vector<bts::Slice> slices = patient->getOrginalData()->getSlices(bts::modalityMap["Flair"]);

		// Do optimal thresholding
		float maxIntensity = float(patient->getOrginalData()->getIntensityMax(bts::modalityMap["Flair"]));

		std::vector<bts::Slice> thresholdedSlices = bts::doOptimalThreshold(slices, 10, 1.5, 50, 1 / (float)maxIntensity, true);

		// Get the biggest continuos region  using the growing region
		processedSlices = getBiggestConnectedRegion(thresholdedSlices);		

		// *** deprecated version ***
		// Calculate the seed/centroid
		/*cv::Point3i centroid = getCentroid(thresholdedSlices);

		// Patch the seed
		//if (thresholdedSlices.at(centroid.z).getData().at<float>(centroid.y, centroid.x) != 1.0)
		//{
		//centroid = findNearestPositive(thresholdedSlices, centroid);
		//}
		if (centroid.x == -1)
		{
			processedSlices = thresholdedSlices;
		}
		else{
			// Growing region
			processedSlices = bts::growingRegion(thresholdedSlices, centroid, 0);
		}*/

		// Set up the results
		bts::ProcessedData *processedData = new bts::ProcessedData();
		processedData->setSliceCount(slices.size());
		processedData->setSlices(processedSlices); 
		processedData->setPatient(patient);
		processedData->setTitle("Complex segmentation: " + patient->getPatientId());

		// Evaluate the new processed data
		std::vector<bts::Slice> slicesGT;
		slicesGT = patient->getOrginalData()->getSlices(bts::modalityMap["GT"]);
		processedData->evaluate(slicesGT);

		// Store the processed data
		std::vector<bts::ProcessedData> pd = patient->getProcessedData();
		pd.push_back(*processedData);
		patient->setProcessedData(pd);
	}

	cv::Point3i findNearestPositive(std::vector<bts::Slice> mask, cv::Point3i centroid)
	{
		int width = mask.at(0).getData().cols;
		int height = mask.at(0).getData().rows;
		int *nxp, *nyp, *nzp;
		int ncount;
		// 4-neighbourhood in 3D
		ncount = 6;
		int nx[] = { -1, 0, 0, 0, 0, 1 };
		int ny[] = { 0, 0, 0, -1, 1, 0 };
		int nz[] = { 0, -1, 1, 0, 0, 0 };
		nxp = nx;
		nyp = ny;
		nzp = nz;

		std::vector<cv::Point3i> queue;
		queue.push_back(centroid);
		mask.at(centroid.z).getData().at<float>(centroid.y, centroid.x) = -1;

		while (!queue.empty())
		{
			// get voxel from queue
			cv::Point3i curVox = queue.front();
			queue.erase(queue.begin());

			for (int i = 0; i < ncount; i++)
			{
				cv::Point3i nPoint = cv::Point3i(curVox.x + nxp[i], curVox.y + nyp[i], curVox.z + nzp[i]);
				float value = mask.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x);
				if (nPoint.x < 0 || nPoint.x >= width ||
					nPoint.y < 0 || nPoint.y >= height ||
					nPoint.z < 0 || nPoint.z >= mask.size() ||
					value == -1)
				{
					continue;
				}

				if (value == 1.0)
				{
					return nPoint;
				}

				queue.push_back(nPoint);
				// mark as visited
				mask.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x) = -1;
			}

		}
	}

	std::vector<bts::Slice> growingRegion(std::vector<bts::Slice> mask, cv::Point3i seed, int connectivity)
	{
		// Growing region
		int width = mask.at(0).getData().cols;
		int height = mask.at(0).getData().rows;
		std::vector<bts::Slice> processedSlices = bts::doThreshold(mask, 100.0, 1.0); // just for initialization
		std::vector<cv::Point3i> queue;
		queue.push_back(seed);

		int *nxp, *nyp, *nzp;
		int ncount;
		if (connectivity == 0)
		{
			// 4-neighbourhood in 3D
			ncount = 6;
			int nx[] = { -1, 0, 0, 0, 0, 1 };
			int ny[] = { 0, 0, 0, -1, 1, 0 };
			int nz[] = { 0, -1, 1, 0, 0, 0 };
			nxp = nx;
			nyp = ny;
			nzp = nz;
		}
		else
		{
			ncount = 26;
			// 8-neighbourhood in 3D
			int nx[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			int ny[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1 };
			int nz[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1 };
			nxp = nx;
			nyp = ny;
			nzp = nz;
		}

		while (!queue.empty())
		{
			// get voxel from queue
			cv::Point3i curVox = queue.front();
			queue.erase(queue.begin());

			// check whether valid voxel
			if (mask.at(curVox.z).getData().at<float>(curVox.y, curVox.x) == 1.0)
			{
				// The seed grows only in the mask
				processedSlices.at(curVox.z).getData().at<float>(curVox.y, curVox.x) = 1.0;

				// find neighbours
				for (int i = 0; i < ncount; i++)
				{
					cv::Point3i nPoint = cv::Point3i(curVox.x + nxp[i], curVox.y + nyp[i], curVox.z + nzp[i]);
					if (nPoint.x < 0 || nPoint.x >= width ||
						nPoint.y < 0 || nPoint.y >= height ||
						nPoint.z < 0 || nPoint.z >= mask.size() ||
						processedSlices.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x) == 1.0 ||
						processedSlices.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x) == 0.5)
					{
						continue;
					}
					queue.push_back(nPoint);
					processedSlices.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x) = 0.5; // next to visit
				}
			}
		}
		processedSlices = bts::doThreshold(processedSlices, 0.9, 1.0); // Because of evaluation, need to be binary image

		return processedSlices;
	}

	cv::Point3i getCentroid(std::vector<Slice> slices)
	{
		// Calculate the seed/centroid
		int width = slices.at(0).getData().cols;
		int height = slices.at(0).getData().rows;
		long x = 0, y = 0, z = 0;
		int count = 0;
		double intensity = 0.0;

		for (int i = 0; i < slices.size(); i++)
		{
			bts::Slice slice = slices.at(i);
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					if (slice.getData().at<float>(j, k) == 1)
					{
						count++;
						x += k;
						y += j;
						z += i;
					}
				}
			}
		}
		if (count == 0)
		{
			x = y = z = -1;
		}
		else
		{
			x /= count;
			y /= count;
			z /= count;
		}

		return cv::Point3i(x, y, z);
	}

	int getPatientByName(std::vector<Patient> patients, std::string name)
	{
		for (int i = 0; i < patients.size(); i++)
		{
			// find the prior patient object
			if (name.compare(patients.at(i).getPatientId()) == 0)
			{
				return i;
			}
		}
		return -1;
	}

	cv::Mat createHistogram(cv::Mat img, int bins)
	{
		// convert image to 16U with scale "bins"
		img.convertTo(img, CV_16U, bins);

		// Initalize histogram
		cv::Mat hist;
		hist = cv::Mat::zeros(1, bins, CV_32SC1);

		// Calculate the histogram of the image
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				ushort val = img.at<ushort>(i, j);
				if (val > 0 && val < bins) // ignoring black background
				{
					hist.at<int>(val) += 1;
				}
			}
		}

		return hist;
	}

	void showHistogram(cv::Mat hist, std::string winTitle)
	{
		cv::Mat canvas;
		int bins = hist.size().width;
		double maxValue;
		cv::minMaxLoc(hist, NULL, &maxValue);

		// Display histogram in a canvas
		canvas = cv::Mat::ones(500, bins, CV_8UC3);

		for (int j = 0, rows = canvas.rows; j < bins - 1; j++)
		{

			line(
				canvas,
				cv::Point(j, rows),
				cv::Point(j, rows - (hist.at<int>(j) * rows / maxValue)),
				cv::Scalar(200, 200, 200),
				1, 8, 0
				);
		}
		cv::imshow(winTitle.c_str(), canvas);
	}

	int countPeaks(cv::Mat hist)
	{
		int countPeaks = 0;
		for (int i = 1; i < hist.size().width-1; i++)
		{
			int a = hist.at<int>(i-1);
			int b = hist.at<int>(i); 
			int c = hist.at<int>(i + 1);
			if (a < b)
			{
				if (b>c)
				{
					countPeaks++;
				}
				else if (b==c)
				{
					for (i++; i < hist.size().width - 1; i++)
					{
						c = hist.at<int>(i + 1);
						if (b!=c)
						{
							if (b>c)
							{
								countPeaks++;
							}
							break;
						}
					}
				}

			}
		}
		return countPeaks;
	}

	cv::Mat averageHistogram(cv::Mat hist, int distance)
	{
		// average the whole histogram
		cv::Mat meanHist;
		int bins = hist.size().width;
		meanHist = cv::Mat::zeros(1, bins, CV_32SC1);
		int begin, end;
		for (int i = 1; i < bins; i++)
		{
			begin = i - distance;
			begin = begin < 1 ? 1 : begin;
			end = i + distance;
			end = end >= bins ? bins - 1 : end;
			int mCount = 0, mSum = 0;
			for (int j = begin; j <= end; j++)
			{
				mCount++;
				mSum += hist.at<int>(j);
			}
			meanHist.at<int>(i) = round(mSum / (float)mCount);
		}
		return meanHist;
	}

	float findOptimalThresholdValue(cv::Mat img, int angle, float distance, int fromTop, float nf)
	{
		// set number of histogram bins - precision
		int bins = 1 / nf;

		// TODO vyrieö to ûe ak je to processed data tak nastav vhodn˝ poËet binov, aby histogram bol s˙visl˝
		// dalo by sa tak, ûe spravi veæ˝m poËtom inov histogram zistÌ sa ûe koæko z toho je 0 a podæa toho pomeru potom znÌûiù poËet binov
		// napr. ak nastavÌm na 5000 a 50% n˙l, tak nastavÌm poËet binov na 2500
		if (bins == 1)
		{
			bins = 1000;
		}

		// Create histogram with specific number of bins
		cv::Mat hist = createHistogram(img, bins);

		/******************************TODO test*/
		/*cv::Mat smoothHist = hist.clone();

		smoothHist = averageHistogram(smoothHist,bins*0.015);
		int peakCount = countPeaks(smoothHist);
		showHistogram(smoothHist,std::to_string(peakCount));*/
			/***************************************/

		// Average the histogram
		hist = averageHistogram(hist, bins*0.015);


		// For histogram, obtain the maximum (peak) value and index 
		int maxIndex = 0;
		double maxValue = 0;

		for (int j = 0; j < bins - 1; j++){
			if (hist.at<int>(j) >= maxValue)
			{
				maxValue = hist.at<int>(j);
				maxIndex = j;
			}
		}

		// find the threshold value
		int x1 = 0, x2 = 0;
		float alfa;
		int binDistance = bins * distance / 100.0;
		for (int i = maxIndex; i < bins - binDistance; i++)
		{
			x1 = i;
			x2 = i + binDistance;
			alfa = atan(binDistance / float(hist.at<int>(x1) -hist.at<int>(x2)));
			alfa = M_PI / 2.0 - alfa;
			if (alfa < (M_PI / 180.0) * angle && (hist.at<int>(x1) / maxValue) * 100 < fromTop)
			{
				break;
			}
		}
		//showHistogram(hist, std::to_string(x1));

		return x1 / (float)bins;
	}

	std::vector<Slice> doOptimalThreshold(std::vector<Slice> slices, int angle, float distance, int fromTop, float nf, bool minAvg)
	{
		std::vector<Slice> processedSlices;
		std::vector<float> thresholdValues;
		float avgThresholdValue;
		if (minAvg == true) // calculate all optimal threshold values and calculate average
		{
			thresholdValues = findOptimalThresholdValues(slices, angle, distance, fromTop, nf);
			int zeros = std::count(thresholdValues.begin(), thresholdValues.end(), 0.0); // TODO vyskuöaj Ëi nie je lepöie ak sa pravi average celeho a pripocita sa + 10% otestuj to na vöetkych pacientoch ci sa nepokazi TP velmi
			avgThresholdValue = std::accumulate(thresholdValues.begin(), thresholdValues.end(), 0.0) / float(thresholdValues.size() - zeros);
		}
		for (int i = 0; i < slices.size(); i++)
		{
			Slice slice = slices.at(i);
			//TODO bins
			cv::Mat inputImage = slice.getData();
			cv::Mat outputImage;
			inputImage.convertTo(inputImage, CV_32F, nf);
			float thresholdValue;
			if (minAvg)
			{
				thresholdValue = thresholdValues.at(i);
				if (thresholdValue < avgThresholdValue)
				{
					thresholdValue = avgThresholdValue;
				}
			}
			else
			{
				thresholdValue = findOptimalThresholdValue(inputImage, angle, distance, fromTop, nf);
			}
			cv::threshold(inputImage, outputImage, thresholdValue, 1.0, CV_THRESH_BINARY);

			slice.setData(outputImage);
			processedSlices.push_back(slice);
		}
		return processedSlices;
	}

	std::vector<float> findOptimalThresholdValues(std::vector<Slice> slices, int angle, float distance, int fromTop, float nf)
	{
		std::vector<float> thresholdValues;
		int countBad = 0;
		for (int i = 0; i < slices.size(); i++)
		{
			cv::Mat img = slices.at(i).getData();
			img.convertTo(img, CV_32F, nf);
			float thresholdValue = findOptimalThresholdValue(img, angle, distance, fromTop, nf);

			/*****************/
			double maxValue;
			cv::minMaxLoc(img, NULL, &maxValue);
			if (maxValue > 0.5 && maxValue*0.80 < thresholdValue)
			{
				countBad++;
			}
			/*****************/
			thresholdValues.push_back(thresholdValue);
		}
		std::ofstream outfile;
		outfile.open("BadData.txt", std::ios_base::app);
		outfile << countBad << "\n";
		outfile.close();

		return thresholdValues;
	}

	std::vector<Slice> doThreshold(std::vector<Slice> slices, float thresholdValue, float normalizationFactor)
	{
		std::vector<Slice> processedSlices;
		for (int i = 0; i < slices.size(); i++)
		{
			bts::Slice slice = slices.at(i);
			cv::Mat inputImage, outputImage;
			slice.getData().convertTo(inputImage, CV_32F, normalizationFactor);
			cv::threshold(inputImage, outputImage, thresholdValue, 1.0, CV_THRESH_BINARY);

			slice.setData(outputImage);
			processedSlices.push_back(slice);
		}
		return processedSlices;
	}

	bool isProcessedData(std::string name)
	{
		for (std::map<std::string, int>::iterator it = modalityMap.begin(); it != modalityMap.end(); ++it)
		{
			if (name.compare(it->first) == 0)
			{
				return false;
			}
		}
		return true;
	}

	std::vector<bts::Slice> substruct(std::vector<bts::Slice> slicesA, std::vector<bts::Slice> slicesB, float nfA, float nfB)
	{
		for (int i = 0; i < slicesA.size(); i++)
		{
			cv::Mat imgA = slicesA.at(i).getData();
			cv::Mat imgB = slicesB.at(i).getData();
			imgA.convertTo(imgA, CV_32F, nfA);
			imgB.convertTo(imgB, CV_32F, nfB);

			cv::Mat result = imgA - imgB;
			cv::threshold(result, result, 0, 1, CV_THRESH_TOZERO);
			slicesA.at(i).setData(result);
		}
		return slicesA;
	}

	float *** convertSlicesTo3DArray(std::vector<Slice> slices)
	{
		float ***data = new float**[155];
		for (int i = 0; i < 155; ++i)
		{
			data[i] = new float*[240];
			for (int j = 0; j < 240; ++j)
			{
				data[i][j] = new float[240];
			}
		}

		float *sliceData = (float*)slices.at(0).getData().data;

		slices.at(0).getData().at<float>(1, 1);

		return data;
	}

	std::vector<Slice> calculateSuperpixels(const std::vector<bts::Slice>& slices, const std::vector<bts::Slice>& gtSlices, float nf, std::map<std::string, float> *configuration)
	{
		// Configuration variables with default values
		spxCompactness = 0.05;
		spxSideSize = 16;
		iterations = 5;
		bool enforceConnectivity = true;
		std::vector<bool> calculateFeatures = { true/*calculateFeatures?*/, true/*MeanHistogram*/, true/*MeanHistogramWithNeighbours*/, true/*Entropy*/, true/*Location - Angle and Distance*/ };
		int colorSpace = 2; // 0 - CIELab, 1 - XYZ, 2 - RGB
		bool onlyFlairFeatures = false;
		trainingPurpose = false;


		// Get and set selected configurations
		float value;
		std::map<std::string, float>::iterator it;
		spxCompactness = (it = configuration->find("compactness")) != configuration->end() ? it->second : spxCompactness;
		spxSideSize = (it = configuration->find("spxSideSize")) != configuration->end() ? it->second : spxSideSize;
		iterations = (it = configuration->find("iterations")) != configuration->end() ? it->second : iterations;
		enforceConnectivity = (it = configuration->find("enforceConnectivity")) != configuration->end() ? it->second : enforceConnectivity; // Dangerous, in case of precision, must be exact 0 for false
		colorSpace = (it = configuration->find("colorSpace")) != configuration->end() ? it->second : colorSpace;
		onlyFlairFeatures = (it = configuration->find("onlyFlairFeatures")) != configuration->end() ? it->second : onlyFlairFeatures; // Dangerous, in case of precision, must be exact 0 for false
		trainingPurpose = (it = configuration->find("trainingPurpose")) != configuration->end() ? it->second : trainingPurpose;

		int featuresCount = 4;
		for (int i = 0; i <= featuresCount; i++)
		{
			calculateFeatures.at(i) = (it = configuration->find("features" + std::to_string(i))) != configuration->end() ? it->second : calculateFeatures.at(i);
		}

		std::vector<Slice> processedSlices;

		// Getting local max value (per sequence) - in case of global max value the results are worse because of big intensity differences between different sequences
		
		// gSLICr settings
		gSLICr::objects::settings mySettings;
		mySettings.img_size.x = slices.at(0).getData().cols;
		mySettings.img_size.y = slices.at(0).getData().rows;
		mySettings.spixel_size = spxSideSize;
		mySettings.coh_weight = spxCompactness;
		mySettings.no_iters = iterations;
		mySettings.color_space = (gSLICr::COLOR_SPACE) colorSpace;
		mySettings.seg_method = gSLICr::GIVEN_SIZE;
		mySettings.do_enforce_connectivity = enforceConnectivity;

		// instantiate a core_engine
		gSLICr::engines::core_engine* gSLICrEngine = new gSLICr::engines::core_engine(mySettings);

		// gSLICr takes gSLICr::UChar4Image as input and out put
		gSLICr::UChar4Image* inImg = new gSLICr::UChar4Image(mySettings.img_size, true, true);
		gSLICr::UChar4Image* outImg = new gSLICr::UChar4Image(mySettings.img_size, true, true);
		cv::Size s(mySettings.img_size.x, mySettings.img_size.y);

		// Calculate the superpixels for each slice
		for (int i = 0; i < slices.size(); i++)
		{
			cv::Mat boundaryDrawFrame;
			boundaryDrawFrame.create(s, CV_8UC1);
			Slice slice = slices.at(i);

			cv::Mat frame = slice.getData();

			frame.convertTo(frame, CV_8UC1, nf * 255); // TODO str·came tu presnosù

			gSLICrLoadImage(frame, inImg);

			gSLICrEngine->Process_Frame(inImg);

			gSLICrEngine->Draw_Segmentation_Result(outImg);

			gSLICrLoadImage(outImg, boundaryDrawFrame);

			// Get the mask
			const gSLICr::IntImage *mask = gSLICrEngine->Get_Seg_Res();
			const int* mask_ptr = mask->GetData(MEMORYDEVICE_CPU);

			// TODO preco nepouzivam tento maskMat pre CalculateFetures kde sa vypocitava vector superpixelov z masky, bolo by to sikovnejsie, prerob to!!!
			cv::Mat maskMat;
			maskMat.create(s, CV_32SC1);
			for (int y = 0; y < mySettings.img_size.y; y++)
			for (int x = 0; x < mySettings.img_size.x; x++)
			{
				int idx = x + y * mySettings.img_size.x;
				maskMat.at<int>(y, x) = mask_ptr[idx];
			}

			slice.setData(boundaryDrawFrame);
			slice.setSpxMask(maskMat);

			processedSlices.push_back(slice);
			// Calculate the features
			if (calculateFeatures.at(0))
			{
				// Get superpixels count
				int countSPX = mask_ptr[0];
				for (int j = 1; j < mySettings.img_size.x * mySettings.img_size.y; j++)
				{
					countSPX = max(countSPX, mask_ptr[j]);
				}
				countSPX++;

				// Get the centroid of the volume
				std::vector<bts::Slice> thresholdedSlices = bts::doThreshold(slices, 1, 1);
				cv::Point3i centroid = getCentroid(thresholdedSlices);
				if (centroid.x == -1)
				{
					continue;
				}

				// Get ground truth image
				cv::Mat gtImage = gtSlices.at(i).getData();

				std::vector<std::vector<float>> features;
				std::vector<bts::Slice> fSlices;
				std::vector<float> fNFs;

				// TODO prerobit calculateFeatures, aby ti vratilo vypocitane features a potom to vypisat do suboru az tu vonku! Potom by stacilo pocitat tumorousFeatures iba tu
				fSlices.push_back(slices.at(i));
				fNFs.push_back(nf);
				calculateFeaturesOfSuperpixels(fSlices, gtImage, mask_ptr, countSPX, fNFs, centroid, calculateFeatures, features);

				if (!evaluateSPX && features.size() != 0)
				{
					// Store the superpixels features to file
					std::string fileName = "superpixel_train_4features.txt"; // TODO dynamic name
					saveSuperpixelsFeaturesToFile(features, fileName);
				}

			}
		}
		return processedSlices;
			
	}

	std::vector<Slice> calculateSuperpixels(bts::Patient* patient, std::map<std::string, float> *configuration)
	{
		// Configuration variables with default values
		spxCompactness = 0.05;
		spxSideSize = 16;
		iterations = 5;
		bool enforceConnectivity = true;
		std::vector<bool> calculateFeatures = { true/*calculateFeatures?*/, true/*MeanHistogram*/, true/*MeanHistogramWithNeighbours*/, true/*Entropy*/, true/*Location - Angle and Distance*/ };
		int colorSpace = 2; // 0 - CIELab, 1 - XYZ, 2 - RGB
		bool onlyFlairSLIC = false;
		bool onlyFlairFeatures = false;
		trainingPurpose = false;


		// Get and set selected configurations
		float value;
		std::map<std::string, float>::iterator it;
		spxCompactness = (it = configuration->find("compactness")) != configuration->end() ? it->second : spxCompactness;
		spxSideSize = (it = configuration->find("spxSideSize")) != configuration->end() ? it->second : spxSideSize;
		iterations = (it = configuration->find("iterations")) != configuration->end() ? it->second : iterations;
		enforceConnectivity = (it = configuration->find("enforceConnectivity")) != configuration->end() ? it->second : enforceConnectivity; // Dangerous, in case of precision, must be exact 0 for false
		colorSpace = (it = configuration->find("colorSpace")) != configuration->end() ? it->second : colorSpace;
		onlyFlairSLIC = (it = configuration->find("onlyFlairSLIC")) != configuration->end() ? it->second : onlyFlairSLIC; // Dangerous, in case of precision, must be exact 0 for false
		onlyFlairFeatures = (it = configuration->find("onlyFlairFeatures")) != configuration->end() ? it->second : onlyFlairFeatures; // Dangerous, in case of precision, must be exact 0 for false
		trainingPurpose = (it = configuration->find("trainingPurpose")) != configuration->end() ? it->second : trainingPurpose;

		int featuresCount = 4;
		for (int i = 0; i <= featuresCount; i++)
		{
			calculateFeatures.at(i) = (it = configuration->find("features" + std::to_string(i))) != configuration->end() ? it->second : calculateFeatures.at(i);
		}


		std::vector<Slice> processedSlices;

		// We leave out T1 sequence becuase it contains the less information about tumor
		std::vector<bts::Slice> slicesFLAIR = patient->getOrginalData()->getSlices(bts::modalityMap["Flair"]);
		std::vector<bts::Slice> slicesT1c = patient->getOrginalData()->getSlices(bts::modalityMap["T1c"]);
		std::vector<bts::Slice> slicesT2 = patient->getOrginalData()->getSlices(bts::modalityMap["T2"]);
		std::vector<bts::Slice> slicesGT = patient->getOrginalData()->getSlices(bts::modalityMap["GT"]);

		// Getting local max value (per sequence) - in case of global max value the results are worse because of big intensity differences between different sequences
		float nfFLAIR = 1 / float(patient->getOrginalData()->getIntensityMax(bts::modalityMap["Flair"]));
		float nfT1c = 1 / float(patient->getOrginalData()->getIntensityMax(bts::modalityMap["T1c"]));
		float nfT2 = 1 / float(patient->getOrginalData()->getIntensityMax(bts::modalityMap["T2"]));

		// gSLICr settings
		gSLICr::objects::settings mySettings;
		mySettings.img_size.x = slicesFLAIR.at(0).getData().cols;
		mySettings.img_size.y = slicesFLAIR.at(0).getData().rows;
		mySettings.spixel_size = spxSideSize;
		mySettings.coh_weight = spxCompactness;
		mySettings.no_iters = iterations;
		mySettings.color_space = (gSLICr::COLOR_SPACE) colorSpace;// gSLICr::RGB;//gSLICr::CIELAB; 
		mySettings.seg_method = gSLICr::GIVEN_SIZE;
		mySettings.do_enforce_connectivity = enforceConnectivity;

		// instantiate a core_engine
		gSLICr::engines::core_engine* gSLICrEngine = new gSLICr::engines::core_engine(mySettings);

		// gSLICr takes gSLICr::UChar4Image as input and out put
		gSLICr::UChar4Image* inImg = new gSLICr::UChar4Image(mySettings.img_size, true, true);
		gSLICr::UChar4Image* outImg = new gSLICr::UChar4Image(mySettings.img_size, true, true);
		cv::Size s(mySettings.img_size.x, mySettings.img_size.y);

		// Calculate the superpixels for each slice
		for (int i = 0; i < slicesFLAIR.size(); i++)
		{
			cv::Mat boundaryDrawFrame;
			boundaryDrawFrame.create(s, CV_8UC1);
			Slice slice = slicesFLAIR.at(i);

			cv::Mat frame1 = slicesFLAIR.at(i).getData();
			cv::Mat frame2 = slicesT1c.at(i).getData();
			cv::Mat frame3 = slicesT2.at(i).getData();

			if (onlyFlairSLIC)
			{
				frame1.convertTo(frame1, CV_8UC1, nfFLAIR * 255); // TODO str·came tu presnosù

				gSLICrLoadImage(frame1, inImg);
			}
			else
			{
				frame1.convertTo(frame1, CV_8UC1, nfFLAIR * 255); // TODO str·came tu presnosù
				frame2.convertTo(frame2, CV_8UC1, nfT1c * 255); // TODO str·came tu presnosù
				frame3.convertTo(frame3, CV_8UC1, nfT2 * 255); // TODO str·came tu presnosù

				gSLICrLoadImage(frame1, frame2, frame3, inImg);
			}

			gSLICrEngine->Process_Frame(inImg);

			gSLICrEngine->Draw_Segmentation_Result(outImg);

			gSLICrLoadImage(outImg, boundaryDrawFrame);

			// Get the mask
			const gSLICr::IntImage *mask = gSLICrEngine->Get_Seg_Res();
			const int* mask_ptr = mask->GetData(MEMORYDEVICE_CPU);

			// TODO preco nepouzivam tento maskMat pre CalculateFetures kde sa vypocitava vector superpixelov z masky, bolo by to sikovnejsie, prerob to!!!
			cv::Mat maskMat;
			maskMat.create(s, CV_32SC1);
			for (int y = 0; y < mySettings.img_size.y; y++)
			for (int x = 0; x < mySettings.img_size.x; x++)
			{
				int idx = x + y * mySettings.img_size.x;
				maskMat.at<int>(y, x) = mask_ptr[idx];
			}
			
			slice.setData(boundaryDrawFrame);
			slice.setSpxMask(maskMat);
			

			/*************************TESTOVANIE MAX KVALITY SEGMENTACIE NA ZAKLADE SUPERPIXELOV***************************/
			/*// Get superpixels count
			int countSPX = mask_ptr[0];
			for (int j = 1; j < mySettings.img_size.x * mySettings.img_size.y; j++)
			{
				countSPX = max(countSPX, mask_ptr[j]);
			}

			countSPX++;
			std::vector<std::set<int>> neighbourhood(countSPX);

			// get superpixels from the current mask
			std::vector<std::vector<cv::Point2i>> spxLoc = getSuperpixelsFromMask(mask_ptr, neighbourhood, countSPX, mySettings.img_size.x, mySettings.img_size.y);
			for (int j = 0; j < spxLoc.size(); j++)
			{
				int result = isTumour(spxLoc.at(j), slicesGT.at(i).getData(), evaluateSPX);
				maskMat.convertTo(maskMat, CV_32F, 1.0);
				if (spxLoc.at(j).size() == 0) continue;
				int label = maskMat.at<float>(spxLoc.at(j).at(0).y, spxLoc.at(j).at(0).x);
				maskMat.setTo(result, maskMat == label);
			}
			
			slice.setData(maskMat);*/

			/**************************************************************************************************************/
			processedSlices.push_back(slice);
			// Calculate the features
			if (calculateFeatures.at(0))
			{
				// Get superpixels count
				int countSPX = mask_ptr[0];
				for (int j = 1; j < mySettings.img_size.x * mySettings.img_size.y; j++)
				{
					countSPX = max(countSPX, mask_ptr[j]);
				}
				countSPX++;

				// Get the centroid of the volume
				std::vector<bts::Slice> thresholdedSlices = bts::doThreshold(slicesFLAIR, 1, 1);
				cv::Point3i centroid = getCentroid(thresholdedSlices);
				if (centroid.x == -1)
				{
					continue;
				}

				// Get ground truth image
				cv::Mat gtImage = slicesGT.at(i).getData();

				std::vector<std::vector<float>> features;
				std::vector<bts::Slice> fSlices;
				std::vector<float> fNFs;
				// TODO prerobit calculateFeatures, aby ti vratilo vypocitane features a potom to vypisat do suboru az tu vonku! Potom by stacilo pocitat tumorousFeatures iba tu
				if (onlyFlairFeatures)
				{
					fSlices.push_back(slicesFLAIR.at(i));
					fNFs.push_back(nfFLAIR);
					calculateFeaturesOfSuperpixels(fSlices, gtImage, mask_ptr, countSPX, fNFs, centroid, calculateFeatures, features);
					/*calculateFeaturesOfSuperpixels(slicesFLAIR.at(i), gtImage, mask_ptr, countSPX, nfFLAIR, centroid, calculateFeatures, tumorousFeatures, nonTumorousfeatures);*/
				}
				else
				{
					fSlices.push_back(slicesFLAIR.at(i));
					fSlices.push_back(slicesT1c.at(i));
					fSlices.push_back(slicesT2.at(i));
					fNFs.push_back(nfFLAIR);
					fNFs.push_back(nfT1c);
					fNFs.push_back(nfT2);

					calculateFeaturesOfSuperpixels(fSlices, gtImage, mask_ptr, countSPX, fNFs, centroid, calculateFeatures, features);
					/*calculateFeaturesOfSuperpixels(slicesFLAIR.at(i), gtImage, mask_ptr, countSPX, nfFLAIR, centroid, calculateFeatures, tumorousFeatures, nonTumorousfeatures);
					calculateFeaturesOfSuperpixels(slicesT1c.at(i), gtImage, mask_ptr, countSPX, nfT1c, centroid, calculateFeatures, tumorousFeatures, nonTumorousfeatures);
					calculateFeaturesOfSuperpixels(slicesT2.at(i), gtImage, mask_ptr, countSPX, nfT2, centroid, calculateFeatures, tumorousFeatures, nonTumorousfeatures);*/
				}

				if (!evaluateSPX && features.size() != 0)
				{
					// Store the superpixels features to file
					std::string fileName = "superpixel_train_4features.txt"; // TODO dynamic name
					saveSuperpixelsFeaturesToFile(features, fileName);
				}

			}
		}
		
		return processedSlices;

	}

	void saveSuperpixelsFeaturesToFile(std::vector<std::vector<float>>& features, std::string fileName)
	{
		std::ofstream outfile;
		outfile.open(fileName, std::ios_base::app);

		int featuresDim = features.at(0).size();

		int tCount = 0;
		int hCount = 0;
		std::string labels;
		for (int i = 0; i < features.size(); i++)
		{
			if (features.at(i).at(0))  // tumor or healthy
			{
				labels = "0 1";
				tCount++;
			}
			else
			{
				labels = "1 0";
				hCount++;
			}

			outfile << "|labels " << labels << " |features ";
			for (int j = 1; j < featuresDim; j++)
			{
				outfile << features.at(i).at(j) << " ";
			}
			outfile << "\n";
		}

		// In case of training purpose we need to keep the ratio of tumorous and nontumorous superpixels to 1:1 TODO
		if (false/*trainingPurpose*/)
		{
			float multiplier = hCount / (float)tCount - 1;
			for (int i = 0; i < multiplier*tCount; i++)
			{
				outfile << "|labels " << labels << " |features ";
				for (int j = 0; j < featuresDim; j++)
				{
					outfile << features.at(i%tCount).at(j) << " ";
				}
				outfile << "\n";
			}
		}

		outfile.close();
	}

	void calculateFeaturesOfSuperpixels(const std::vector<bts::Slice>& fSlices, const cv::Mat& gt, const int* mask, int noSPX, const std::vector<float>& fNFs, cv::Point3i centroid, std::vector<bool> calculateFeatures, std::vector<std::vector<float>>& features)
	{
		const int bins = 12; // Histogram bins
		int cols = gt.cols;
		int rows = gt.rows;
		int noSlices = fSlices.at(0).getNumber();
		std::vector<std::set<int>> neighbourhood(noSPX);
		std::vector<std::vector<cv::Point2i>> spxLoc = getSuperpixelsFromMask(mask, neighbourhood, noSPX, cols, rows);
		std::vector<std::vector<std::vector<int>>> allSPXIntesities = getAllPixelIntensitiesOfSuperpixels(fSlices, spxLoc);

		// Iterate trough all sequences slice
		for (int sequenceIndex = 0; sequenceIndex < fSlices.size(); sequenceIndex++)
		{
			std::vector<std::vector<int>> spxIntensities = allSPXIntesities.at(sequenceIndex);
			float nf = fNFs.at(sequenceIndex);
			float idxc = nf * (bins - 1); // indexing coefficient for histogram

			// Extract features from each superpixels individually
			for (int i = 0, validSPX = 0; i < spxLoc.size(); i++)
			{
				if (spxLoc.at(i).size() == 0) // Some superpixels disappear, they have no assigned pixels
				{
					continue;
				}

				// ignoring superpixels of black background - all pixels with 0 intensity
				cv::Point2i spxCentroid = getCentroid(spxLoc.at(i));
				float euclideanDistance = sqrt(pow(spxCentroid.x - centroid.x, 2) +
					pow(spxCentroid.y - centroid.y, 2) +
					pow((noSlices - 1) - centroid.z, 2));

				float distThreshold = sqrt(rows * cols / 16.0); // should be 60.0 for our data

				// TODO treba prerobit AND pre vsetky // TODO only when first one, and fill the vector of validity
				if (trainingPurpose) // TODO mozno by bolo aj pri trenovani pouzit taketo superpixle, je to otaznee
				{
					//bool skipIt = (isAllIntesitiesLess(1 / idxc, spxIntensities.at(i), 0.80) && euclideanDistance > distThreshold);
					bool skipIt = true; // TODO prerobit aby to nemuselo robit po kazdom, ale len raz, cize tie vysledky treba ulozit do vektora a potom uz kontrolovat len to. TODO toto cele by sa dalo spravit mimo tohto loopu cize najprv vsetko vypocitat
					for (int kk = 0; kk < fSlices.size(); kk++)
					{
						float idxc2 = fNFs.at(kk) * (bins - 1);
						skipIt = (skipIt && (isAllIntesitiesLess(1 / idxc2, allSPXIntesities.at(kk).at(i), 0.80) && euclideanDistance > distThreshold));
						if (!skipIt)
						{
							break;
						}
					}
					if (skipIt)
					{
						continue;
					}
				}

				std::vector<float> featureNumbers;

				// Save classification label to file;  1 - tumour or 0 - non-tumour
				int result = isTumour(spxLoc.at(i), gt, evaluateSPX);  //TODO ak budes chcieù klasifikovat jednotlive casti nadora, daj vypocotej majoritnej casti a dopln o dalsie vektory

				// In case of SPX quality evaluation do not calculate the features to save time
				if (evaluateSPX)
				{
					continue;
				}

				// Calculate features and store the results to vectors

				// 1 - Intensity histogram feature
				int *histogram = nullptr;
				float *normHist = nullptr;
				int numel = 0;
				if (calculateFeatures.at(1) || calculateFeatures.at(2) || calculateFeatures.at(3))
				{
					histogram = new int[bins];
					normHist = new float[bins];
					memset(histogram, 0, bins*sizeof(int));
					memset(normHist, 0, bins*sizeof(int));
					numel = spxLoc.at(i).size();
					for (int j = 0; j < spxLoc.at(i).size(); j++)
					{
						histogram[int(idxc*spxIntensities.at(i).at(j))]++;
					}
					// Normalize histogram
					int max = 0;
					for (int j = 0; j < bins; j++)
					{
						if (histogram[j]>max)
						{
							max = histogram[j];
						}
					}
					for (int j = 0; j < bins; j++)
					{
						normHist[j] = histogram[j] / (float)max;
					}

					if (calculateFeatures.at(1))
					{
						// Save to vector
						for (int j = 0; j < bins; j++)
						{
							featureNumbers.push_back(normHist[j]);
						}
					}
				}

				// 2 - Neighbourhood intensity histogram feature
				if (calculateFeatures.at(2) || calculateFeatures.at(3))
				{
					std::set<int> neighbours = neighbourhood.at(i);
					for (std::set<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
					{
						//std::vector<cv::Point2i> neighbourSPXLoc = spxLoc.at(*it);
						std::vector<int> neighbourSPXIntensities = spxIntensities.at(*it);
						numel += neighbourSPXIntensities.size();
						for (int j = 0; j < neighbourSPXIntensities.size(); j++)
						{
							histogram[int(idxc*neighbourSPXIntensities.at(j))]++;
						}
					}

					// Normalize histogram
					int max = 0;
					for (int j = 0; j < bins; j++)
					{
						if (histogram[j]>max)
						{
							max = histogram[j];
						}
					}
					for (int j = 0; j < bins; j++)
					{
						normHist[j] = histogram[j] / (float)max;
					}

					if (calculateFeatures.at(2))
					{
						// Save to vector
						for (int j = 0; j < bins; j++)
						{
							featureNumbers.push_back(normHist[j]);
						}
					}
				}

				// 3 - Entropy of neighbours
				if (calculateFeatures.at(3))
				{
					float e = entropy(histogram, bins, numel);
					// Save to vector
					featureNumbers.push_back(e);
				}

				// 4 - Location feature
				if (calculateFeatures.at(4))
				{
					// radians
					double angleXY = atan2(spxCentroid.x - centroid.x, spxCentroid.y - centroid.y);
					double angleXZ = atan2(spxCentroid.x - centroid.x, (noSlices - 1) - centroid.z);
					double angleYZ = atan2(spxCentroid.y - centroid.y, (noSlices - 1) - centroid.z);
					angleXY = angleXY < 0 ? angleXY + (2 * PI) : angleXY;
					angleXZ = angleXZ < 0 ? angleXZ + (2 * PI) : angleXZ;
					angleYZ = angleYZ < 0 ? angleYZ + (2 * PI) : angleYZ;

					// Save to vector
					featureNumbers.push_back(angleXY);
					featureNumbers.push_back(angleXZ);
					featureNumbers.push_back(angleYZ);
					featureNumbers.push_back(euclideanDistance);
				}

				if (sequenceIndex != 0)
				{
					features.at(validSPX).insert(features.at(validSPX).end(), featureNumbers.begin(), featureNumbers.end());
					validSPX++; // To know with which superpixel we are dealing with
				}
				else
				{
					featureNumbers.insert(featureNumbers.begin(), result);
					features.push_back(featureNumbers);
				}
				
			}
		}
		/*
				// Iterate trough all sequences slice
		for (int sliceIndex = 0; sliceIndex < fSlices.size(); sliceIndex++)
		{
			bts::Slice slice = fSlices.at(sliceIndex);
			cv::Mat sliceImg = slice.getData();
			sliceImg.convertTo(sliceImg, CV_16U, 1);
			std::vector<std::vector<cv::Point3i>> superpixels(noSPX); // x, y - position, z - intensity value
			std::vector<std::set<int>> neighbourhood(noSPX);
			const int bins = 12; // Histogram bins
			float nf = fNFs.at(sliceIndex);
			float idxc = nf * (bins - 1); // indexing coefficient for histogram

			short dx[4] = { -1, 0, 1, 0 };
			short dy[4] = { 0, 1, 0, -1 };
			for (int y = 0; y < sliceImg.rows; y++)
			for (int x = 0; x < sliceImg.cols; x++)
			{
				int idx = x + y * sliceImg.cols;
				// Extract superpixels from mask // TODO toto by bolo tiez dobre vyhodit do funkcie, raz vypocitat a hotovo, problem tu vsak robi neighbourhood, takze spravit dva typy funkcii jedna iba pre extrakciu a jedna s neighbours
				cv::Point3i pixel(x, y, sliceImg.at<ushort>(y, x));
				superpixels.at(mask[idx]).push_back(pixel);

				// calculating neighbours
				for (int i = 0; i < 4; i++)
				{
					int idxN = (x + dx[i]) + (y + dy[i]) * sliceImg.cols;
					if (mask[idx] != mask[idxN])
					{
						neighbourhood.at(mask[idx]).insert(mask[idxN]);
						neighbourhood.at(mask[idxN]).insert(mask[idx]);
					}
				}
			}

			// Extract features from each superpixels individually
			for (int i = 0, validHSPX = 0, validTSPX = 0; i < superpixels.size(); i++)
			{
				std::vector<cv::Point3i> superpixel = superpixels.at(i);
				if (superpixel.size() == 0) // Some superpixels disappear, they have no assigned pixels
				{
					continue;
				}

				// ignoring superpixels of black background - all pixels with 0 intensity
				cv::Point2i spxCentroid = getCentroid(superpixel);
				float euclideanDistance = sqrt(pow(spxCentroid.x - centroid.x, 2) +
					pow(spxCentroid.y - centroid.y, 2) +
					pow((slice.getNumber() - 1) - centroid.z, 2));

				float distThreshold = sqrt(sliceImg.rows * sliceImg.cols / 16.0); // should be 60.0 for our data

				
				if (trainingPurpose && isAllIntesitiesLess(1 / idxc, superpixel, 0.80) && euclideanDistance > distThreshold) // TODO mozno by bolo aj pri trenovani pouzit taketo superpixle, je to otaznee
				{
					continue;
				}

				std::vector<float> featureNumbers;

				// Save classification label to file;  1 - tumour or 0 - non-tumour
				int result = isTumour(superpixel, gt, evaluateSPX);  //TODO ak budes chcieù klasifikovat jednotlive casti nadora, daj vypocotej majoritnej casti a dopln o dalsie vektory

				// In case of SPX quality evaluation do not calculate the features to save time
				if (evaluateSPX)
				{
					continue;
				}

				// Calculate features and store the results to vectors

				// 1 - Intensity histogram feature
				int *histogram = nullptr;
				float *normHist = nullptr;
				int numel = 0;
				if (calculateFeatures.at(1) || calculateFeatures.at(2) || calculateFeatures.at(3))
				{
					histogram = new int[bins];
					normHist = new float[bins];
					memset(histogram, 0, bins*sizeof(int));
					memset(normHist, 0, bins*sizeof(int));
					numel = superpixel.size();
					for (int j = 0; j < superpixel.size(); j++)
					{
						histogram[int(idxc*superpixel.at(j).z)]++;
					}
					// Normalize histogram
					int max = 0;
					for (int j = 0; j < bins; j++)
					{
						if (histogram[j]>max)
						{
							max = histogram[j];
						}
					}
					for (int j = 0; j < bins; j++)
					{
						normHist[j] = histogram[j] / (float)max;
					}

					if (calculateFeatures.at(1))
					{
						// Save to vector
						for (int j = 0; j < bins; j++)
						{
							featureNumbers.push_back(normHist[j]);
						}
					}
				}

				// 2 - Neighbourhood intensity histogram feature
				if (calculateFeatures.at(2) || calculateFeatures.at(3))
				{
					std::set<int> neighbours = neighbourhood.at(i);
					for (std::set<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
					{
						std::vector<cv::Point3i> neighbourSuperpixel = superpixels.at(*it);
						numel += neighbourSuperpixel.size();
						for (int j = 0; j < neighbourSuperpixel.size(); j++)
						{
							histogram[int(idxc*neighbourSuperpixel.at(j).z)]++;
						}
					}

					// Normalize histogram
					int max = 0;
					for (int j = 0; j < bins; j++)
					{
						if (histogram[j]>max)
						{
							max = histogram[j];
						}
					}
					for (int j = 0; j < bins; j++)
					{
						normHist[j] = histogram[j] / (float)max;
					}

					if (calculateFeatures.at(2))
					{
						// Save to vector
						for (int j = 0; j < bins; j++)
						{
							featureNumbers.push_back(normHist[j]);
						}
					}
				}

				// 3 - Entropy of neighbours
				if (calculateFeatures.at(3))
				{
					float e = entropy(histogram, bins, numel);
					// Save to vector
					featureNumbers.push_back(e);
				}

				// 4 - Location feature
				if (calculateFeatures.at(4))
				{
					// radians
					double angleXY = atan2(spxCentroid.x - centroid.x, spxCentroid.y - centroid.y);
					double angleXZ = atan2(spxCentroid.x - centroid.x, (slice.getNumber() - 1) - centroid.z);
					double angleYZ = atan2(spxCentroid.y - centroid.y, (slice.getNumber() - 1) - centroid.z);
					angleXY = angleXY < 0 ? angleXY + (2 * PI) : angleXY;
					angleXZ = angleXZ < 0 ? angleXZ + (2 * PI) : angleXZ;
					angleYZ = angleYZ < 0 ? angleYZ + (2 * PI) : angleYZ;

					// Save to vector
					featureNumbers.push_back(angleXY);
					featureNumbers.push_back(angleXZ);
					featureNumbers.push_back(angleYZ);
					featureNumbers.push_back(euclideanDistance);
				}

				if (sliceIndex != 0)
				{
					if (result)
					{
						tumorousFeatures.at(validTSPX).insert(tumorousFeatures.at(validTSPX).end(), featureNumbers.begin(), featureNumbers.end());
						validTSPX++; // To know with which tumorous superpixel we are dealing with
					}
					else
					{
						nonTumorousFeatures.at(validHSPX).insert(nonTumorousFeatures.at(validHSPX).end(), featureNumbers.begin(), featureNumbers.end());
						validHSPX++; // To know with which healty superpixel we are dealing with
					}
				}
				else
				{
					if (result)
					{
						tumorousFeatures.push_back(featureNumbers);
					}
					else
					{
						nonTumorousFeatures.push_back(featureNumbers);
					}
				}
				
			}
		}*/
	}
	void calculateFeaturesOfSuperpixels(const Slice& slice, const cv::Mat& gt, const int* mask, int noSPX, float nf, cv::Point3i centroid, std::vector<bool> calculateFeatures, std::vector<std::vector<float>>& features)
	{
	/*	cv::Mat sliceImg = slice.getData();
		sliceImg.convertTo(sliceImg, CV_16U, 1);
		std::vector<std::vector<cv::Point3i>> superpixels(noSPX); // x, y - position, z - intensity value
		std::vector<std::set<int>> neighbourhood(noSPX);
		const int bins = 12; // Histogram bins
		float idxc = nf * (bins - 1); // indexing coefficient for histogram

		short dx[4] = { -1, 0, 1, 0 };
		short dy[4] = { 0, 1, 0, -1 };
		for (int y = 0; y < sliceImg.rows; y++)
		for (int x = 0; x < sliceImg.cols; x++)
		{
			int idx = x + y * sliceImg.cols;
			// Extract superpixels from mask // TODO toto by bolo tiez dobre vyhodit do funkcie, raz vypocitat a hotovo, problem tu vsak robi neighbourhood, takze spravit dva typy funkcii jedna iba pre extrakciu a jedna s neighbours
			cv::Point3i pixel(x, y, sliceImg.at<ushort>(y, x));
			superpixels.at(mask[idx]).push_back(pixel);

			// calculating neighbours
			for (int i = 0; i < 4; i++)
			{
				int idxN = (x + dx[i]) + (y + dy[i]) * sliceImg.cols;
				if (mask[idx] != mask[idxN])
				{
					neighbourhood.at(mask[idx]).insert(mask[idxN]);
					neighbourhood.at(mask[idxN]).insert(mask[idx]);
				}
			}
		}

		// Extract features from each superpixels individually
		for (int i = 0; i < superpixels.size(); i++)
		{
			std::vector<cv::Point3i> superpixel = superpixels.at(i);
			if (superpixel.size() == 0) // Some superpixels disappear, they have no assigned pixels
			{
				continue;
			}

			// ignoring superpixels of black background - all pixels with 0 intensity
			cv::Point2i spxCentroid = getCentroid(superpixel);
			float euclideanDistance = sqrt(pow(spxCentroid.x - centroid.x, 2) +
				pow(spxCentroid.y - centroid.y, 2) +
				pow((slice.getNumber() - 1) - centroid.z, 2));

			float distThreshold = sqrt(sliceImg.rows * sliceImg.cols / 16.0); // should be 60.0 for our data


			if (trainingPurpose && isAllIntesitiesLess(1 / idxc, superpixel, 0.80) && euclideanDistance > distThreshold) // TODO mozno by bolo aj pri trenovani pouzit taketo superpixle, je to otaznee
			{
				continue;
			}

			std::vector<float> featureNumbers;

			// Save classification label to file;  1 - tumour or 0 - non-tumour
			int result = isTumour(superpixel, gt, evaluateSPX);  //TODO ak budes chcieù klasifikovat jednotlive casti nadora, daj vypocotej majoritnej casti a dopln o dalsie vektory

			// In case of SPX quality evaluation do not calculate the features to save time
			if (evaluateSPX)
			{
				continue;
			}

			// Calculate features and store the results to vectors

			// 1 - Intensity histogram feature
			int *histogram = nullptr;
			float *normHist = nullptr;
			int numel = 0;
			if (calculateFeatures.at(1) || calculateFeatures.at(2) || calculateFeatures.at(3))
			{
				histogram = new int[bins];
				normHist = new float[bins];
				memset(histogram, 0, bins*sizeof(int));
				memset(normHist, 0, bins*sizeof(int));
				numel = superpixel.size();
				for (int j = 0; j < superpixel.size(); j++)
				{
					histogram[int(idxc*superpixel.at(j).z)]++;
				}
				// Normalize histogram
				int max = 0;
				for (int j = 0; j < bins; j++)
				{
					if (histogram[j]>max)
					{
						max = histogram[j];
					}
				}
				for (int j = 0; j < bins; j++)
				{
					normHist[j] = histogram[j] / (float)max;
				}

				if (calculateFeatures.at(1))
				{
					// Save to vector
					for (int j = 0; j < bins; j++)
					{
						featureNumbers.push_back(normHist[j]);
					}
				}
			}

			// 2 - Neighbourhood intensity histogram feature
			if (calculateFeatures.at(2) || calculateFeatures.at(3))
			{
				std::set<int> neighbours = neighbourhood.at(i);
				for (std::set<int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
				{
					std::vector<cv::Point3i> neighbourSuperpixel = superpixels.at(*it);
					numel += neighbourSuperpixel.size();
					for (int j = 0; j < neighbourSuperpixel.size(); j++)
					{
						histogram[int(idxc*neighbourSuperpixel.at(j).z)]++;
					}
				}

				// Normalize histogram
				int max = 0;
				for (int j = 0; j < bins; j++)
				{
					if (histogram[j]>max)
					{
						max = histogram[j];
					}
				}
				for (int j = 0; j < bins; j++)
				{
					normHist[j] = histogram[j] / (float)max;
				}

				if (calculateFeatures.at(2))
				{
					// Save to vector
					for (int j = 0; j < bins; j++)
					{
						featureNumbers.push_back(normHist[j]);
					}
				}
			}

			// 3 - Entropy of neighbours
			if (calculateFeatures.at(3))
			{
				float e = entropy(histogram, bins, numel);
				// Save to vector
				featureNumbers.push_back(e);
			}

			// 4 - Location feature
			if (calculateFeatures.at(4))
			{
				// radians
				double angleXY = atan2(spxCentroid.x - centroid.x, spxCentroid.y - centroid.y);
				double angleXZ = atan2(spxCentroid.x - centroid.x, (slice.getNumber() - 1) - centroid.z);
				double angleYZ = atan2(spxCentroid.y - centroid.y, (slice.getNumber() - 1) - centroid.z);
				angleXY = angleXY < 0 ? angleXY + (2 * PI) : angleXY;
				angleXZ = angleXZ < 0 ? angleXZ + (2 * PI) : angleXZ;
				angleYZ = angleYZ < 0 ? angleYZ + (2 * PI) : angleYZ;

				// Save to vector
				featureNumbers.push_back(angleXY);
				featureNumbers.push_back(angleXZ);
				featureNumbers.push_back(angleYZ);
				featureNumbers.push_back(euclideanDistance);
			}

			if (result)
			{
				tumorousFeatures.push_back(featureNumbers);
			}
			else
			{
				nonTumorousFeatures.push_back(featureNumbers);
			}
		}*/
	}
	
	float entropy(int *histogram, int bins, int numel)
	{
		float entropy = 0;
		if (numel == 0)
		{
			for (int i = 0; i < bins; i++)
			{
				numel += histogram[i];
			}
		}

		for (int i = 0; i < bins; i++)
		{
			// Make probability distribution (the sum is 1)
			float prob = histogram[i] / (float)numel;

			// remove zero entries because of log
			if (prob != 0)
			{
				//E = -sum(p.*log2(p));
				entropy += prob * log2(prob);
			}
		}
		entropy *= -1;
		return entropy;
	}

	int isTumour(std::vector<cv::Point2i> superpixel, const cv::Mat& gt, bool calculateQuality)
	{
		std::ofstream outfile;
		std::string fileName = "_UndersegmentationError_" + std::to_string(spxSideSize) + "_" + std::to_string(spxCompactness) + ".txt";
		outfile.open(fileName, std::ios_base::app);
		int count = 0;
		for (int i = 0; i < superpixel.size(); i++)
		{
			if (gt.at<ushort>(superpixel.at(i).y, superpixel.at(i).x) > 0)
			{
				count++;
			}
		}
		float tumourRatio = count / (float)superpixel.size();

		// Undersegmentation error - only involed to GT
		if (count > 0) // at least one tumour voxel
		{
			outfile << min(count, superpixel.size() - count) << ' ' << count << "\n";
		}

		if (tumourRatio >= 0.50)
		{
			return 1;
		}
		else{
			return 0;
		}

		outfile.close();
	}

	// Cannot be used for superpixels with size equal to zero
	cv::Point2i getCentroid(std::vector<cv::Point2i> superpixel)
	{
		long x = 0, y = 0;

		for (int i = 0; i < superpixel.size(); i++)
		{
			x += superpixel.at(i).x;
			y += superpixel.at(i).y;
		}

		x /= superpixel.size();
		y /= superpixel.size();

		return cv::Point2i(x, y);
	}

	bool isAllIntesitiesLess(int threshold, std::vector<int> superpixel)
	{
		for (int i = 0; i < superpixel.size(); i++)
		{
			if (superpixel.at(i) < threshold)
			{
				continue;
			}
			return false;
		}
		return true;
	}

	bool isAllIntesitiesLess(int threshold, std::vector<int> superpixel, float percentage)
	{
		int higherThreshold = (1 - percentage) * superpixel.size();
		int countHigher = 0;
		for (int i = 0; i < superpixel.size(); i++)
		{
			if (superpixel.at(i) < threshold)
			{
				continue;
			}
			else if (++countHigher >= higherThreshold)
			{
				return false;
			}
		}
		return true;
	}

	// Convert from cv::Mat to gSLICr::Uchar4Image
	void gSLICrLoadImage(const cv::Mat& inimg, gSLICr::UChar4Image* outimg)
	{
		gSLICr::Vector4u* outimg_ptr = outimg->GetData(MEMORYDEVICE_CPU);

		for (int y = 0; y < outimg->noDims.y; y++)
		for (int x = 0; x < outimg->noDims.x; x++)
		{
			int idx = x + y * outimg->noDims.x;
			outimg_ptr[idx].b = inimg.at<uchar>(y, x);
			outimg_ptr[idx].g = inimg.at<uchar>(y, x);
			outimg_ptr[idx].r = inimg.at<uchar>(y, x);
		}
	}

	// Convert from cv::Mat to gSLICr::Uchar4Image - 3 channels
	void gSLICrLoadImage(const cv::Mat& one, const cv::Mat& two, const cv::Mat& three, gSLICr::UChar4Image* outimg)
	{
		gSLICr::Vector4u* outimg_ptr = outimg->GetData(MEMORYDEVICE_CPU);

		for (int y = 0; y < outimg->noDims.y; y++)
		for (int x = 0; x < outimg->noDims.x; x++)
		{
			int idx = x + y * outimg->noDims.x;
			outimg_ptr[idx].b = one.at<uchar>(y, x);
			outimg_ptr[idx].g = two.at<uchar>(y, x);
			outimg_ptr[idx].r = three.at<uchar>(y, x);
		}
	}

	// Convert from gSLICr::Uchar4Image to cv::Mat
	void gSLICrLoadImage(const gSLICr::UChar4Image* inimg, cv::Mat& outimg)
	{
		const gSLICr::Vector4u* inimg_ptr = inimg->GetData(MEMORYDEVICE_CPU);

		for (int y = 0; y < inimg->noDims.y; y++)
		for (int x = 0; x < inimg->noDims.x; x++)
		{
			int idx = x + y * inimg->noDims.x;
			outimg.at<uchar>(y, x) = inimg_ptr[idx].b;
		}
	}

	// Convert from gSLICr::IntImage to cv::Mat
	void gSLICrLoadImage(const gSLICr::IntImage* inimg, cv::Mat& outimg)
	{
		const int* inimg_ptr = inimg->GetData(MEMORYDEVICE_CPU);

		for (int y = 0; y < inimg->noDims.y; y++)
		for (int x = 0; x < inimg->noDims.x; x++)
		{
			int idx = x + y * inimg->noDims.x;
			outimg.at<uchar>(y, x) = inimg_ptr[idx] * 4;
		}
	}

	std::vector<std::vector<cv::Point2i>> getSuperpixelsFromMask(const int* mask, std::vector<std::set<int>>& neighbourhood, const int noSPX, int cols, int rows)
	{
		std::vector<std::vector<cv::Point2i>> spxLoc(noSPX);

		short dx[4] = { -1, 0, 1, 0 };
		short dy[4] = { 0, 1, 0, -1 };
		for (int y = 0; y < rows; y++)
		for (int x = 0; x < cols; x++)
		{
			int idx = x + y * cols;
			// Extract superpixels from mask
			cv::Point2i pixel(x, y);
			spxLoc.at(mask[idx]).push_back(pixel);

			// calculating neighbours
			for (int i = 0; i < 4; i++)
			{
				int idxN = (x + dx[i]) + (y + dy[i]) * cols;
				if (mask[idx] != mask[idxN])
				{
					neighbourhood.at(mask[idx]).insert(mask[idxN]);
					neighbourhood.at(mask[idxN]).insert(mask[idx]);
				}
			}
		}
		return spxLoc;
	}

	std::vector<std::vector<std::vector<int>>> getAllPixelIntensitiesOfSuperpixels(const std::vector<bts::Slice>& fSlices, const std::vector<std::vector<cv::Point2i>>& spxLoc)
	{
		std::vector<std::vector<std::vector<int>>> spxIntensities(fSlices.size(), std::vector<std::vector<int>>(spxLoc.size()));

		// Loop superpixels
		for (int i = 0; i < spxLoc.size(); i++)
		{
			std::vector<cv::Point2i> spx = spxLoc.at(i);
			// Loop pixels
			for (int j = 0; j < spx.size(); j++)
			{
				cv::Point2i px = spx.at(j);
				for (int k = 0; k < fSlices.size(); k++)
				{
					spxIntensities.at(k).at(i).push_back(fSlices.at(k).getData().at<ushort>(px.y, px.x));
				}

			}
		}
		return spxIntensities;
	}
}

