#include "thresholdtoolwindow.h"
#include "ui_thresholdtoolwindow.h"
#include <iomanip>

ThresholdToolWindow::ThresholdToolWindow(std::vector<bts::Patient> &patients) :
ui(new Ui::ThresholdToolWindow),
patients(&patients)
{
	ui->setupUi(this);

	if (patients.size() == 0)
	{
		return;
	}

	ui->pushButtonOK->setEnabled(true);
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

ThresholdToolWindow::~ThresholdToolWindow()
{
    delete ui;
}


void ThresholdToolWindow::on_pushButtonOK_clicked()
{
	std::vector<bts::Slice> slices;
	bts::ProcessedData *processedData = new bts::ProcessedData();
	float nf = 1.0;
	float thresholdValue;
	std::string processingTitle = ui->comboBoxSlices->currentText().toStdString() + " thresh" + " [";


	// is it orginal data or processed data
	int currentSlicesIndex = ui->comboBoxSlices->currentIndex();
	if (currentSlicesIndex < processedDataStart) // org data
	{
		// get modality
		int modalityIndex = bts::modalityMap[ui->comboBoxSlices->currentText().toStdString()];
		processedData->setModality(modalityIndex);
		nf = 1 / float(currentPatient->getOrginalData()->getIntensityMax(modalityIndex)); //TODO add to GUI a choice of global or local normalization 
		//nf = 1 / float(currentPatient->getOrginalData()->getGlobalIntensityMax());
		slices = currentPatient->getOrginalData()->getSlices(modalityIndex);
	}
	else // processed data
	{
		bts::ProcessedData oldProcessedData = currentPatient->getProcessedData().at(currentSlicesIndex - processedDataStart);
		processedData->setModality(oldProcessedData.getModality());
		slices = oldProcessedData.getSlices();
	}

	if (ui->checkBoxFindOptimal->isChecked())
	{
		processedData->setSlices(bts::doOptimalThreshold(slices, ui->spinBoxAngle->value(), ui->doubleSpinBoxDistance->value(), ui->spinBoxFromTop->value(), nf, ui->checkBoxMinAverage->isChecked()));		

		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << ui->doubleSpinBoxDistance->value();
		
		processingTitle += std::to_string(ui->spinBoxAngle->value()) + "Deg, ";
		processingTitle += stream.str() +"\%D, ";
		processingTitle += std::to_string(ui->spinBoxFromTop->value()) + "\%FT, ";
		processingTitle += QString::number(ui->checkBoxMinAverage->isChecked()).toStdString() +" ]";
	}
	else
	{
		thresholdValue = ui->spinBoxThreshold->value() / 100.0;
		// process and save images
		processedData->setSlices(bts::doThreshold(slices, thresholdValue, nf));
		processingTitle +=  QString::number(thresholdValue).toStdString() + "]";
	}

	// set title of the processing
	processedData->setTitle(processingTitle);

	// set slice count
	processedData->setSliceCount(slices.size());

	// set patient for processedData
	processedData->setPatient(currentPatient);

	// Evaluate the new processed data
	std::vector<bts::Slice> slicesGT;
	slicesGT = currentPatient->getOrginalData()->getSlices(bts::modalityMap["GT"]);
	processedData->evaluate(slicesGT);

	// set the processedData to the currentPatient
	std::vector<bts::ProcessedData> pd = currentPatient->getProcessedData();
	pd.push_back(*processedData);
	currentPatient->setProcessedData(pd);


	this->close();
}

void ThresholdToolWindow::on_pushButtonCancel_clicked()
{
	this->close();
}

void ThresholdToolWindow::fillSlicesComboBox(bts::Patient patient)
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

void ThresholdToolWindow::on_comboBoxPatient_currentTextChanged(const QString &arg1)
{
	for(int i = 0; i < patients->size(); i++)
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

void ThresholdToolWindow::on_checkBoxFindOptimal_stateChanged(int arg1)
{
	if (ui->checkBoxFindOptimal->isChecked())
	{
		ui->spinBoxAngle->setEnabled(true);
		ui->spinBoxFromTop->setEnabled(true);
		ui->doubleSpinBoxDistance->setEnabled(true);
		ui->checkBoxMinAverage->setEnabled(true);

		ui->spinBoxThreshold->setEnabled(false);
	}
	else
	{
		ui->spinBoxAngle->setEnabled(false);
		ui->spinBoxFromTop->setEnabled(false);
		ui->doubleSpinBoxDistance->setEnabled(false);
		ui->checkBoxMinAverage->setEnabled(false);

		ui->spinBoxThreshold->setEnabled(true);
	}
}
