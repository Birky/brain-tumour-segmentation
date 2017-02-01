#include "superpixelisationwindow.h"
#include "ui_superpixelisationwindow.h"
#include <iomanip>

/*SuperpixelisationWindow::SuperpixelisationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuperpixelisationWindow)
{
    ui->setupUi(this);
}*/

SuperpixelisationWindow::SuperpixelisationWindow(std::vector<bts::Patient> &patients) :
ui(new Ui::SuperpixelisationWindow),
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

	currentPatient = &(patients.at(0));
	fillSlicesComboBox(*currentPatient);

	ui->comboBoxPatient->addItems(patientList);
}

SuperpixelisationWindow::~SuperpixelisationWindow()
{
    delete ui;
}

void SuperpixelisationWindow::on_radioButtonAll_clicked()
{
	ui->checkBoxT1->setEnabled(true);
	ui->checkBoxT1c->setEnabled(true);
	ui->checkBoxT2->setEnabled(true);
	ui->checkBoxFlair->setEnabled(true);

	ui->comboBoxPatient->setEnabled(false);
	ui->comboBoxSlices->setEnabled(false);
}

void SuperpixelisationWindow::on_radioButtonSpecific_clicked()
{
	ui->checkBoxT1->setEnabled(false);
	ui->checkBoxT1c->setEnabled(false);
	ui->checkBoxT2->setEnabled(false);
	ui->checkBoxFlair->setEnabled(false);

	ui->comboBoxPatient->setEnabled(true);
	ui->comboBoxSlices->setEnabled(true);
}

void SuperpixelisationWindow::on_checkBoxFeatures_stateChanged(int arg1)
{
	if (arg1 == 0)
	{ 
		ui->groupBoxFeatures->setEnabled(false);
	}
	else
	{
		ui->groupBoxFeatures->setEnabled(true);
	}
}

void SuperpixelisationWindow::fillSlicesComboBox(bts::Patient patient)
{
	ui->comboBoxSlices->clear();
	QStringList modalityList;
	// add all modalities
	for (std::map<std::string, int>::iterator it = bts::modalityMap.begin(); it != bts::modalityMap.end(); ++it)
	{
		if (patient.getOrginalData()->getSlices(it->second).size() > 0)
		{
			modalityList.push_back(QString::fromStdString(it->first));
		}
	}

	std::vector<bts::ProcessedData> pd = patient.getProcessedData();
	// add all processsed data
	processedDataStart = modalityList.size(); // needed for processedData detection
	for (int i = 0; i < pd.size(); i++)
	{
		modalityList.push_back(QString::fromStdString(pd.at(i).getTitle()));
	}

	ui->comboBoxSlices->addItems(modalityList);
}
void SuperpixelisationWindow::on_buttonBox_accepted()
{
	// Get specified data only
	if (ui->radioButtonSpecific->isChecked())
	{
		superpixelisationSpecificData();
	}
	// Get loaded all data
	else{
		superpixelisationAllData();
	}
}

void SuperpixelisationWindow::on_comboBoxPatient_currentTextChanged(const QString &arg1)
{
	for (int i = 0; i < patients->size(); i++)
	{
		bts::Patient *patient = &patients->at(i);
		if (QString::compare(QString::fromStdString(patient->getPatientId()), arg1) == 0)
		{
			currentPatient = patient;
			fillSlicesComboBox(*currentPatient);
			break;
		}
	}
}

void SuperpixelisationWindow::superpixelisationSpecificData()
{
	std::vector<bts::Slice> slices;
	bts::ProcessedData *processedData = new bts::ProcessedData();
	float nf = 1.0;
	//float thresholdValue;
	std::string processingTitle = ui->comboBoxSlices->currentText().toStdString() + " SPX" + " [";

	// Get slices to process
	int currentSlicesIndex = ui->comboBoxSlices->currentIndex();
	if (currentSlicesIndex < processedDataStart) // org data
	{
		// get modality
		int modalityIndex = bts::modalityMap[ui->comboBoxSlices->currentText().toStdString()];
		processedData->setModality(modalityIndex);
		nf = 1 / float(currentPatient->getOrginalData()->getIntensityMax(modalityIndex));
		slices = currentPatient->getOrginalData()->getSlices(modalityIndex);
	}
	else // processed data
	{
		bts::ProcessedData oldProcessedData = currentPatient->getProcessedData().at(currentSlicesIndex - processedDataStart);
		processedData->setModality(oldProcessedData.getModality());
		slices = oldProcessedData.getSlices();
	}

	processedData->setSlices(bts::calculateSuperpixels(slices, nf, ui->spinBoxSPXSize->value(), ui->doubleSpinBoxCompactness->value(), ui->spinBoxIterations->value(), ui->checkBoxEnforceConnectivity->isChecked()));

	// TODO zmazaù
	/*std::vector<bts::Slice> aaa = processedData->getSlices();
	for (int i = 0; i < aaa.size(); i++)
	{
		cv::imshow(std::to_string(i).c_str(),aaa.at(i).getData());
	}*/
	
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << ui->doubleSpinBoxCompactness->value();

	processingTitle += "SPX:" +std::to_string(ui->spinBoxSPXSize->value());
	processingTitle += " C:" + stream.str() + "\%D, ";
	processingTitle += " It:" + std::to_string(ui->spinBoxIterations->value());
	processingTitle += " EC:" + QString::number(ui->checkBoxEnforceConnectivity->isChecked()).toStdString() + " ]";

	// set title of the processing
	processedData->setTitle(processingTitle);

	// set slice count
	processedData->setSliceCount(slices.size());

	// set patient for processedData
	processedData->setPatient(currentPatient);

	// set the processedData to the currentPatient
	std::vector<bts::ProcessedData> pd = currentPatient->getProcessedData();
	pd.push_back(*processedData);
	currentPatient->setProcessedData(pd);

	this->close();
}


void SuperpixelisationWindow::superpixelisationAllData()
{

}

