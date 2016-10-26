#include "ProcessedData.h"


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

	for (int i = 0; i < this->slices.size(); i++)
	{
		EvaluatedSlice eSlice;
		cv::Mat gtImg = groundTruthSlices.at(i).getData();
		gtImg.convertTo(gtImg, CV_32F);
		cv::threshold(gtImg, gtImg, 0, 1, CV_THRESH_BINARY);
		cv::Mat img = slices.at(i).getData();
		eSlice.setNumber(slices.at(i).getNumber());

		cv::Mat and;
		cv::bitwise_and(img, gtImg, and);
		
		eSlice.TP = cv::countNonZero(and);
		eSlice.FP = cv::countNonZero(img) - eSlice.TP;
		eSlice.FN = cv::countNonZero(gtImg) - eSlice.TP;
		eSlice.TN = (gtImg.cols * gtImg.rows) - (eSlice.TP + eSlice.FP + eSlice.FN);

		// Jaccard
		float jaccard;
		eSlice.jaccard = eSlice.TP / float(eSlice.TP + eSlice.FP + eSlice.FN);

		// Dice
		float dice;
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
		//cv::namedWindow(std::to_string(i), CV_WINDOW_KEEPRATIO);
		//imshow(std::to_string(i), rgbResult);

		evaluatedSlices.push_back(eSlice);
	}
}