#include "superpixelisationwindow.h"
#include "ui_superpixelisationwindow.h"
#include <iomanip>


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
	ui->radioButtonOnlyFlair->setEnabled(true);
	ui->radioButtonThreeSequences->setEnabled(true);

	ui->comboBoxPatient->setEnabled(false);
	ui->comboBoxSlices->setEnabled(false);
}

void SuperpixelisationWindow::on_radioButtonSpecific_clicked()
{
	ui->radioButtonOnlyFlair->setEnabled(false);
	ui->radioButtonThreeSequences->setEnabled(false);

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
	// Get all loaded data
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

	// wich features to calculate
	std::vector<bool> features;
	features.push_back(ui->checkBoxFeatures->isChecked());
	features.push_back(ui->checkBoxIntHist->isChecked());
	features.push_back(ui->checkBoxIntHistNeigh->isChecked());
	features.push_back(ui->checkBoxEntropy->isChecked());
	features.push_back(ui->checkBoxLoc->isChecked());
	
	std::vector<bts::Slice> gtSlices = currentPatient->getOrginalData()->getSlices(bts::modalityMap["GT"]);

	std::map<std::string, float> config = { { "compactness", ui->doubleSpinBoxCompactness->value() },
	{ "spxSideSize", ui->spinBoxSPXSize->value() },
	{ "iterations", ui->spinBoxIterations->value() },
	{ "enforceConnectivity", ui->checkBoxEnforceConnectivity->isChecked() },
	{ "features0", ui->checkBoxFeatures->isChecked() },
	{ "features1", ui->checkBoxIntHist->isChecked() },
	{ "features2", ui->checkBoxIntHistNeigh->isChecked() },
	{ "features3", ui->checkBoxEntropy->isChecked() },
	{ "features4", ui->checkBoxLoc->isChecked() },
	{ "colorSpace", ui->comboBoxColorSpace->currentIndex() },
	{ "onlyFlairSLIC", ui->radioButtonOnlyFlair->isChecked() },
	{ "onlyFlairFeatures", ui->checkBoxOnlyFlairFeatures->isChecked() } };

	// one sequence 3 times
	processedData->setSlices(bts::calculateSuperpixels(slices, gtSlices, nf, &config));

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << ui->doubleSpinBoxCompactness->value();

	processingTitle += "SPX:" +std::to_string(ui->spinBoxSPXSize->value());
	processingTitle += " C:" + stream.str();
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
	bool evaluateSPX = ui->checkBoxEvaluateSPX->isChecked();
	std::map<std::string, float> config = { { "compactness", ui->doubleSpinBoxCompactness->value() },
	{ "spxSideSize", ui->spinBoxSPXSize->value() },
	{ "iterations", ui->spinBoxIterations->value() },
	{ "enforceConnectivity", ui->checkBoxEnforceConnectivity->isChecked() },
	{ "features0", ui->checkBoxFeatures->isChecked() },
	{ "features1", ui->checkBoxIntHist->isChecked() },
	{ "features2", ui->checkBoxIntHistNeigh->isChecked() },
	{ "features3", ui->checkBoxEntropy->isChecked() },
	{ "features4", ui->checkBoxLoc->isChecked() },
	{ "colorSpace", ui->comboBoxColorSpace->currentIndex() },
	{ "onlyFlairSLIC", ui->radioButtonOnlyFlair->isChecked() },
	{ "onlyFlairFeatures", ui->checkBoxOnlyFlairFeatures->isChecked() },
	{ "trainingPurpose", ui->checkBoxTrainingPurpose->isChecked() }};

	// Make the title of the processed data
	std::string processingTitle = ui->comboBoxSlices->currentText().toStdString() + " SPX" + " [";

	for (int i = 0; i < patients->size(); i++) // loop all patients
	{
		currentPatient = &patients->at(i);
		std::vector<bts::Slice> slices;

		// get the processedData of the currentPatient
		std::vector<bts::ProcessedData> pd = currentPatient->getProcessedData();

		// Only when evaluating the superpixel quality
		if (evaluateSPX)
		{
			const int sideSizes[] = { 6, 8, 12, 16, 24, 32 }; //TODO allow in GUI to set this range 
			const float compactnesses[] = { 0.00, 0.05, 0.20, 0.50 };
			for (int globalC = 0; globalC < 4; globalC++)
			{
				for (int globalS = 0; globalS < 6; globalS++)
				{
					config["compactness"] = compactnesses[globalC];
					config["spxSideSize"] = sideSizes[globalS];
					bts::ProcessedData *processedData = new bts::ProcessedData();
					processedData->setSlices(bts::calculateSuperpixels(currentPatient, &config));

					std::stringstream stream;
					stream << std::fixed << std::setprecision(2) << config["compactness"];
					std::string currentTitle = processingTitle + "SPX:" + std::to_string(int(config["spxSideSize"])) +
						" C:" + stream.str() + " It:" + std::to_string(int(config["iterations"])) +
						" EC:" + std::to_string(int(config["enforceConnectivity"])) + " ]";
					// set slice count
					processedData->setSliceCount(processedData->getSlices().size()); //TODO make a function to set up the whole processedData, DRY principle
					// set title of the processing
					processedData->setTitle(currentTitle);
					// set patient for processedData
					processedData->setPatient(currentPatient);

					// Add new processed data to the vector of processed data
					pd.push_back(*processedData);
				}
			}
		}
		else
		{
			bts::ProcessedData *processedData = new bts::ProcessedData();
			processedData->setSlices(bts::calculateSuperpixels(currentPatient, &config));

			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << config["compactness"];
			std::string currentTitle = processingTitle + "SPX:" + std::to_string(int(config["spxSideSize"])) +
				" C:" + stream.str() + " It:" + std::to_string(int(config["iterations"])) +
				" EC:" + std::to_string(int(config["enforceConnectivity"])) + " ]";
			// set slice count
			processedData->setSliceCount(processedData->getSlices().size());
			// set title of the processing
			processedData->setTitle(currentTitle);
			// set patient for processedData
			processedData->setPatient(currentPatient);

			/*********Docasne pre testovanie kvality MAX klasifikacia*********/
			//processedData->evaluate(currentPatient->getOrginalData()->getSlices(4));
			/******************/
			
			// Add new processed data to the vector of processed data
			pd.push_back(*processedData);
		}
		
		// Set the extented vector of processedData to the current patient
		currentPatient->setProcessedData(pd);
	}

	this->close();
}

