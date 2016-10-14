#include "imagearithmwindow.h"
#include "ui_imagearithmwindow.h"

/*ImageArithmWindow::ImageArithmWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageArithmWindow)
{
    ui->setupUi(this);
}*/

ImageArithmWindow::ImageArithmWindow(std::vector<bts::Patient> &patients) :
ui(new Ui::ImageArithmWindow),
patients(&patients)
{
	ui->setupUi(this);

	if (patients.size() == 0)
	{
		return;
	}

	ui->buttonBox->setEnabled(true);

	// set up comboboxes
	QStringList patientList;
	for each (bts::Patient patient in patients)
	{
		patientList.push_back(QString::fromStdString(patient.getPatientId()));
	}

	ui->patientAComboBox->addItems(patientList);
	ui->patientBComboBox->addItems(patientList);

	//// set up comboboxes
	//QStringList imageList;
	//for each (bts::Patient patient in patients)
	//{
	//	// orginal images
	//	for (std::map<std::string, int>::iterator it = bts::modalityMap.begin(); it != bts::modalityMap.end(); ++it)
	//	{
	//		if (patient.getOrginalData()->getSlices(it->second).size() > 0)
	//		{
	//			imageList.push_back(QString::fromStdString(patient.getPatientId() + " " + it->first));
	//		}
	//	}

	//	// processed images
	//	for each (bts::ProcessedData processedData in patient.getProcessedData())
	//	{
	//		imageList.push_back(QString::fromStdString(patient.getPatientId() + " " + processedData.getTitle()));
	//	}
	//}

	//ui->imageAComboBox->addItems(imageList);
	//ui->imageBComboBox->addItems(imageList);
}


ImageArithmWindow::~ImageArithmWindow()
{
    delete ui;
}

void ImageArithmWindow::on_buttonBox_accepted()
{
	std::vector<bts::Slice> slicesA, slicesB;
	bts::ProcessedData *processedData = new bts::ProcessedData();
	float nfA = 1.0, nfB = 1.0;
	// Get image A
	// is it orginal data or processed data
	int currentSlicesIndex = ui->slicesAComboBox->currentIndex();
	if (currentSlicesIndex < processedDataStart[0]) // org data
	{
		// get modality
		int modalityIndex = bts::modalityMap[ui->slicesAComboBox->currentText().toStdString()];
		nfA = 1 / float(currentPatientA->getOrginalData()->getGlobalIntensityMax());
		slicesA = currentPatientA->getOrginalData()->getSlices(modalityIndex);
	}
	else // processed data
	{
		bts::ProcessedData oldProcessedData = currentPatientA->getProcessedData().at(currentSlicesIndex - processedDataStart[0]);
		slicesA = oldProcessedData.getSlices();
	}

	// Get image B
	// is it orginal data or processed data
	currentSlicesIndex = ui->slicesBComboBox->currentIndex();
	if (currentSlicesIndex < processedDataStart[1]) // org data
	{
		// get modality
		int modalityIndex = bts::modalityMap[ui->slicesBComboBox->currentText().toStdString()];
		nfB = 1 / float(currentPatientB->getOrginalData()->getGlobalIntensityMax());
		slicesB = currentPatientB->getOrginalData()->getSlices(modalityIndex);
	}
	else // processed data
	{
		bts::ProcessedData oldProcessedData = currentPatientB->getProcessedData().at(currentSlicesIndex - processedDataStart[1]);
		slicesB = oldProcessedData.getSlices();
	}


	// Do the operation // TODO este dorobit podrobnejšie doplnit rozne udaje a osetrenia
	std::vector<bts::Slice> slicesResult;
	if (ui->substractionRadioButton->isChecked())
	{
		slicesResult = bts::substruct(slicesA, slicesB, nfA, nfB);
		processedData->setSlices(slicesResult);
		processedData->setTitle("Subtract " + ui->slicesAComboBox->currentText().toStdString() + " - " + ui->slicesBComboBox->currentText().toStdString()); // TODO prerob aby sme vedeli aj pacienta, ale mozno by bolo dobre dovolit operacie len medzi jednym pacientom
	}
	//else if ()

	// set the processedData to the currentPatient // TODO prerobit okno aby bolo mozne vybrat len jedneho pacienta
	std::vector<bts::ProcessedData> pd = currentPatientA->getProcessedData();
	pd.push_back(*processedData);
	currentPatientA->setProcessedData(pd);
}

void ImageArithmWindow::fillSlicesComboBox(bts::Patient patient, int image)
{
	QComboBox* comboBox;
	if (image == 0)
	{
		comboBox = ui->slicesAComboBox;
	}
else
{
	comboBox = ui->slicesBComboBox;
}
	comboBox->clear();
	QStringList slicesList;
	// add all modalities
	for (std::map<std::string, int>::iterator it = bts::modalityMap.begin(); it != bts::modalityMap.end(); ++it)
	{
		if (patient.getOrginalData()->getSlices(it->second).size() > 0)
		{
			slicesList.push_back(QString::fromStdString(it->first));
		}
	}

	std::vector<bts::ProcessedData> pd = patient.getProcessedData();
	// add all processsed data
	processedDataStart[image] = slicesList.size(); // needed for processedData detection
	for (int i = 0; i < pd.size(); i++)
	{
		slicesList.push_back(QString::fromStdString(pd.at(i).getTitle()));
	}

	comboBox->addItems(slicesList);
}

void ImageArithmWindow::on_patientAComboBox_currentTextChanged(const QString &arg1)
{
	for (int i = 0; i < patients->size(); i++)
	{
		bts::Patient *patient = &patients->at(i);
		if (QString::compare(QString::fromStdString(patient->getPatientId()), arg1) == 0)
		{
			currentPatientA = patient;
			fillSlicesComboBox(*currentPatientA, 0);
			break;
		}
	}
}

void ImageArithmWindow::on_patientBComboBox_currentTextChanged(const QString &arg1)
{
	for (int i = 0; i < patients->size(); i++)
	{
		bts::Patient *patient = &patients->at(i);
		if (QString::compare(QString::fromStdString(patient->getPatientId()), arg1) == 0)
		{
			currentPatientB = patient;
			fillSlicesComboBox(*currentPatientB, 1);
			break;
		}
	}
}
