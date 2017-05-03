#include "complexsegmentation.h"
#include "ui_complexsegmentation.h"
#include <intrin.h>

//TODO under construction, finish it for method1
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
	
}

void ComplexSegmentation::on_patientComboBox_currentIndexChanged(int index)
{
	currentPatient = &(patients->at(index));
}

void ComplexSegmentation::on_patientComboBox_currentIndexChanged(const QString &arg1)
{
	bts::doComplexSegmentation(currentPatient);
}
