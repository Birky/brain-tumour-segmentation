#ifndef COMPLEXSEGMENTATION_H
#define COMPLEXSEGMENTATION_H

#include <QDialog>
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"

namespace Ui {
class ComplexSegmentation;
}

class ComplexSegmentation : public QDialog
{
    Q_OBJECT

public:
    //explicit ComplexSegmentation(QWidget *parent = 0);
	ComplexSegmentation(std::vector<bts::Patient> &patients);
	~ComplexSegmentation();

private slots:
    void on_buttonBox_accepted();

    void on_patientComboBox_currentIndexChanged(const QString &arg1);

    void on_patientComboBox_currentIndexChanged(int index);

private:
    Ui::ComplexSegmentation *ui;

	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatient;
};

#endif // COMPLEXSEGMENTATION_H
