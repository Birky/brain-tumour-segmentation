#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(std::vector<bts::Patient> &patients) :
ui(new Ui::ResultWindow),
patients(&patients)
{
	ui->setupUi(this);

	if (patients.size() == 0)
	{
		return;
	}

	// set up comboboxes
	QStringList patientList;
	for each (bts::Patient patient in patients)
	{
		patientList.push_back(QString::fromStdString(patient.getPatientId()));
	}
	ui->comboBoxPatient->addItems(patientList);

	currentPatient = &(patients.at(0));
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

//TODO Add ContextMenu which allows to save the results into a file
void ResultWindow::fillTableWithResults()
{
	ui->tableWidget->clearContents();
	std::string pdName = ui->comboBoxEvaluatedData->currentText().toStdString();
	if (pdName.empty())
	{
		return;
	}
	bts::ProcessedData pd = *(currentPatient->getProcessedData(pdName));
	std::vector<bts::EvaluatedSlice> eSlices = pd.getEvaluatedSlices();

	ui->tableWidget->setRowCount(eSlices.size()+1);
	int totalTP = 0, totalFP = 0, totalFN = 0, totalTN = 0;
	for (int i = 0; i < eSlices.size(); i++)
	{
		ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(eSlices.at(i).getNumber()))); 
		ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(eSlices.at(i).dice)));
		ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(eSlices.at(i).jaccard)));
		ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(eSlices.at(i).TP)));
		ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(eSlices.at(i).FP)));
		ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(eSlices.at(i).FN)));
		ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(eSlices.at(i).TN)));

		totalTP += eSlices.at(i).TP;
		totalFP += eSlices.at(i).FP;
		totalFN += eSlices.at(i).FN;
		totalTN += eSlices.at(i).TN;
	}
	// Jaccard total
	float totalJaccard;
	totalJaccard = totalTP / float(totalTP + totalFP + totalFN);

	// Dice total
	float totalDice;
	long totalTP2 = 2 * totalTP;
	totalDice = totalTP2 / float(totalTP2 + totalFP + totalFN);

	int row = eSlices.size();
	ui->tableWidget->setItem(row, 0, new QTableWidgetItem());
	ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(totalDice)));
	ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(totalJaccard)));
	ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(totalTP)));
	ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(totalFP)));
	ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(totalFN)));
	ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(totalTN)));
}

void ResultWindow::fillEvaluatedDataComboBox(bts::Patient patient)
{
	ui->comboBoxEvaluatedData->clear();
	QStringList processedDataList;
	std::vector<bts::ProcessedData> pd = patient.getProcessedData();

	// add only evaluated processsed data
	for (int i = 0; i < pd.size(); i++)
	{	
		if (pd.at(i).getEvaluatedSlices().size() > 0)
		{
			processedDataList.push_back(QString::fromStdString(pd.at(i).getTitle()));
		}
	}

	ui->comboBoxEvaluatedData->addItems(processedDataList);
}

void ResultWindow::on_comboBoxPatient_currentIndexChanged(int index)
{
	currentPatient = &(patients->at(index));
	fillEvaluatedDataComboBox(*currentPatient);
}

void ResultWindow::on_comboBoxEvaluatedData_currentIndexChanged(const QString &arg1)
{
	fillTableWithResults();
}
