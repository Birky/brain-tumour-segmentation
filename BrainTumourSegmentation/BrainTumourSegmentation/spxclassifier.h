#ifndef SPXCLASSIFIER_H
#define SPXCLASSIFIER_H

#include <QDialog>

#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"

namespace Ui {
class SPXClassifier;
}

class SPXClassifier : public QDialog
{
    Q_OBJECT

public:
	SPXClassifier(std::vector<bts::Patient> &patients);
    ~SPXClassifier();

private slots:

    void on_pushButtonBrowseResults_clicked();

    void on_buttonBox_accepted();

private:
    Ui::SPXClassifier *ui;


	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatient;
};

#endif // SPXCLASSIFIER_H
