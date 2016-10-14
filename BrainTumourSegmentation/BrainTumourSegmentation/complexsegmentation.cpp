#include "complexsegmentation.h"
#include "ui_complexsegmentation.h"
#include <intrin.h>

/*ComplexSegmentation::ComplexSegmentation(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ComplexSegmentation)
	{
	ui->setupUi(this);
	}
	*/

ComplexSegmentation::ComplexSegmentation(std::vector<bts::Patient> &patients) :
ui(new Ui::ComplexSegmentation),
patients(&patients)
{
	ui->setupUi(this);

	if (patients.size() == 0)
	{
		return;
	}

	// set up combobox
	QStringList patientList;
	for each (bts::Patient patient in patients)
	{
		patientList.push_back(QString::fromStdString(patient.getPatientId()));
	}
	ui->patientComboBox->addItems(patientList);

	currentPatient = &(patients.at(0));
}

ComplexSegmentation::~ComplexSegmentation()
{
	delete ui;
}

void ComplexSegmentation::on_buttonBox_accepted()
{
	// get slices of Flair modality
	std::vector<bts::Slice> slices = currentPatient->getOrginalData()->getSlices(bts::modalityMap["Flair"]);
	
	// Do optimal thresholding
	float maxIntensity = currentPatient->getOrginalData()->getGlobalIntensityMax();

/*	// TODO len skusam vyskusaj to po korekcii
	std::vector<bts::Slice> diff = bts::substruct(currentPatient->getOrginalData()->getSlices(bts::modalityMap["Flair"]), currentPatient->getOrginalData()->getSlices(bts::modalityMap["T2"]), 1 / (float)maxIntensity, 1 / (float)maxIntensity);
	std::vector<bts::Slice> slices = bts::substruct(currentPatient->getOrginalData()->getSlices(bts::modalityMap["Flair"]),diff, 1 / (float)maxIntensity, 1.0);


	// TODO delete
	for (int i = 0; i < slices.size(); i++)
	{
		cv::Mat out;
		slices.at(i).getData().convertTo(out,CV_16U,maxIntensity);
		slices.at(i).setData(out);
	}
	*/
	std::vector<bts::Slice> thresholdedSlices = bts::doOptimalThreshold(slices, 10, 1.5, 25, 1 / (float)maxIntensity, true);

	//cv::imshow("diff", diff.at(77).getData());
	cv::imshow("slices", slices.at(77).getData());
	//return;
	// Calculate the seed/centroid
	int width = thresholdedSlices.at(0).getData().cols;
	int height = thresholdedSlices.at(0).getData().rows;
	long x = 0, y = 0, z = 0;
	int count = 0;
	double intensity = 0.0;

	for (int i = 0; i < thresholdedSlices.size(); i++)
	{
		bts::Slice slice = thresholdedSlices.at(i);
		bts::Slice orgSlice = slices.at(i);
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
					intensity += orgSlice.getData().at<unsigned short>(j, k);
				}
			}
		}
	}
	x /= count;
	y /= count;
	z /= count;
	intensity /= (double)count;

	// Growing region
	std::vector<bts::Slice> processedSlices = bts::doThreshold(slices, 100.0, 1.0 / (float)maxIntensity); // just for initialization
	std::vector<cv::Point3i> queue;
	queue.push_back(cv::Point3i(x, y, z));

	// TODO umoûniù cez gui aj neorginalne data dat ako masku

	// TODO umoûniù cez gui obi dva sposoby
	// 4-neighbourhood in 3D
	int nx[] = { -1, 0, 0, 0, 0, 0, 1 };
	int ny[] = { 0, 0, 0, 0, -1, 1, 0 };
	int nz[] = { 0, 0, -1, 1, 0, 0, 0 };

	// 8-neighbourhood in 3D
	/*int nx[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	int ny[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1};
	int nz[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1};
	*/
	while (!queue.empty())
	{
		// get voxel from queue
		cv::Point3i curVox = queue.front();
		queue.erase(queue.begin());

		// check whether valid voxel
		if (thresholdedSlices.at(curVox.z).getData().at<float>(curVox.y, curVox.x) == 1.0)
		{
			// TODO doplni este aj impakt intenzity ci ineho, susedstva napr.
			processedSlices.at(curVox.z).getData().at<float>(curVox.y, curVox.x) = 1.0;
			// find neighbours
			for (int i = 0; i < (sizeof(nz) / sizeof(*nz)); i++)
			{
				cv::Point3i nPoint = cv::Point3i(curVox.x + nx[i], curVox.y + ny[i], curVox.z + nz[i]);
				if (nPoint.x < 0 || nPoint.x >= width ||
					nPoint.y < 0 || nPoint.y >= height ||
					nPoint.z < 0 || nPoint.z >= slices.size() ||
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
	processedSlices = bts::doThreshold(processedSlices, 0.9, 1.0); // Because of evaluation

	// Set up the results
	bts::ProcessedData *processedData = new bts::ProcessedData();
	processedData->setSliceCount(slices.size());
	processedData->setSlices(processedSlices); // TODO 
	processedData->setPatient(currentPatient);
	processedData->setTitle("Complex segmentation: " + currentPatient->getPatientId());

	// Evaluate the new processed data
	std::vector<bts::Slice> slicesGT;
	slicesGT = currentPatient->getOrginalData()->getSlices(bts::modalityMap["GT"]);
	processedData->evaluate(slicesGT);

	// Store the processed data
	std::vector<bts::ProcessedData> pd = currentPatient->getProcessedData();
	pd.push_back(*processedData);
	currentPatient->setProcessedData(pd);
}

void ComplexSegmentation::on_patientComboBox_currentIndexChanged(int index)
{
	currentPatient = &(patients->at(index));
}

void ComplexSegmentation::on_patientComboBox_currentIndexChanged(const QString &arg1)
{

}
