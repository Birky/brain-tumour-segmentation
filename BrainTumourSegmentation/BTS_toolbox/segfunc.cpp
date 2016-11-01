#define _USE_MATH_DEFINES
#include "segfunc.h"
#include "Patient.h"
#include <numeric>
#include <algorithm> 

namespace bts
{
	void doComplexSegmentation(Patient* patient)
	{
		// get slices of Flair modality
		std::vector<bts::Slice> slices = patient->getOrginalData()->getSlices(bts::modalityMap["Flair"]);

		// Do optimal thresholding
		float maxIntensity = patient->getOrginalData()->getGlobalIntensityMax();

		std::vector<bts::Slice> thresholdedSlices = bts::doOptimalThreshold(slices, 10, 1.5, 50, 1 / (float)maxIntensity, true);

		// Calculate the seed/centroid
		cv::Point3i centroid = bts::getCentroid(thresholdedSlices);

		// Growing region
		std::vector<bts::Slice> processedSlices = bts::growingRegion(thresholdedSlices, centroid, 0);

		// Set up the results
		bts::ProcessedData *processedData = new bts::ProcessedData();
		processedData->setSliceCount(slices.size());
		processedData->setSlices(processedSlices); // TODO 
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

	std::vector<bts::Slice> growingRegion(std::vector<bts::Slice> mask, cv::Point3i seed, int connectivity)
	{
		// Growing region
		int width = mask.at(0).getData().cols;
		int height = mask.at(0).getData().rows;
		std::vector<bts::Slice> processedSlices =  bts::doThreshold(mask, 100.0, 1.0); // just for initialization
		std::vector<cv::Point3i> queue;
		queue.push_back(seed);

		// TODO umoûniù cez gui aj neorginalne data dat ako masku
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
				// TODO doplni este aj impakt intenzity ci ineho, susedstva napr.
				processedSlices.at(curVox.z).getData().at<float>(curVox.y, curVox.x) = 1.0;
				int size = sizeof(nzp);
				int size2 = sizeof(*nzp);
				int size3 = sizeof(&nzp);
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
					processedSlices.at(nPoint.z).getData().at<float>(nPoint.y, nPoint.x) = 0.5;
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
			//bts::Slice orgSlice = slices.at(i);
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
						//intensity += orgSlice.getData().at<unsigned short>(j, k);
					}
				}
			}
		}
		if (count == 0)
		{
			x = y = z = 0;
		}
		else
		{
			x /= count;
			y /= count;
			z /= count;
		}
		//intensity /= (double)count;

		return cv::Point3i(x,y,z);
	}

	int getPatientByName(std::vector<Patient> patients, std::string name)
	{
		for(int i = 0; i < patients.size(); i++)
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
			if (alfa < (M_PI / 180.0) * angle && (hist.at<int>(x1) / maxValue)*100 < fromTop)
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
			avgThresholdValue = std::accumulate(thresholdValues.begin(), thresholdValues.end(), 0.0) / float (thresholdValues.size() - zeros);
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
		for (int i = 0; i < slices.size(); i++)
		{
			cv::Mat img = slices.at(i).getData();
			img.convertTo(img, CV_32F, nf);
			float thresholdValue = findOptimalThresholdValue(img, angle, distance, fromTop, nf);
			thresholdValues.push_back(thresholdValue);
		}
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
}

