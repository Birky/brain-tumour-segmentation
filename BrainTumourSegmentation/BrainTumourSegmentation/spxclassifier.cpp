#include "spxclassifier.h"
#include "ui_spxclassifier.h"
#include <QFileDialog>
#include <fstream>

SPXClassifier::SPXClassifier(std::vector<bts::Patient> &patients) :
ui(new Ui::SPXClassifier),
patients(&patients)
{
	ui->setupUi(this);
}


SPXClassifier::~SPXClassifier()
{
    delete ui;
}

void SPXClassifier::on_pushButtonBrowseResults_clicked()
{
	QFileDialog fd;

	fd.setDirectory("D:\\CNTK\\repos\\CNTK\\Examples\\Image\\GettingStarted\\OutputBRATS");
	
	ui->lineEditResults->setText(fd.getOpenFileName());
}

void SPXClassifier::on_buttonBox_accepted()
{
	std::string resultsFileName = ui->lineEditResults->text().toStdString();
	std::ifstream file(resultsFileName);

	for (int i = 0; i < patients->size(); i++)
	{
		bts::Patient* currentPatient = &patients->at(i);

		bts::ProcessedData *processedData = new bts::ProcessedData();
		processedData->setSliceCount(155);
		processedData->setTitle("Deep learning results");
		processedData->setPatient(currentPatient);
			
		//TODO handle to take only superpixel processedData and if any exist show QMessage // now always take the first
		std::vector<bts::Slice> slices =  currentPatient->getProcessedData().at(0).getSlices();

		for (int j = 0; j < slices.size(); j++)
		{
			bts::Slice slice = slices.at(j);
			cv::Mat mask = slice.getSpxMask();

			double min, max;
			cv::minMaxLoc(mask, &min, &max);

			for (int spxIdx = 0; spxIdx <= max; spxIdx++)
			{
				cv::Mat mask2 = mask.clone();
				mask2.setTo(10000, mask2 == spxIdx);

				bool eq = cv::countNonZero(mask != mask2) == 0; // wtf??? TODO presetri, ze co je to tu a co mozes zmazat ale pravdepodobne tu len kontroluje ze ci sa nejedna o superpixel velkosti 0
	
				if (eq)
				{
					continue;
				}

				int classificationResult;
				file >> classificationResult;
				if (classificationResult == 1)
				{
					mask.setTo(10000, mask == spxIdx);
				}
				else
				{
					mask.setTo(0, mask == spxIdx);
				}
			}
			mask.convertTo(mask, CV_32F, 1.0);
			cv::threshold(mask, mask, 0, 1.0, CV_THRESH_BINARY);
			slice.setData(mask.clone());
			slices.at(j) = slice;
		}
		processedData->setSlices(slices);

		std::vector<bts::Slice> slicesGT;
		slicesGT = currentPatient->getOrginalData()->getSlices(bts::modalityMap["GT"]);
		processedData->evaluate(slicesGT);

		std::vector<bts::ProcessedData> pd = currentPatient->getProcessedData();
		pd.push_back(*processedData);
		currentPatient->setProcessedData(pd);		
	
	}
}
