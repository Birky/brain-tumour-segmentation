#ifndef THRESHOLDTOOLWINDOW_H
#define THRESHOLDTOOLWINDOW_H

#include <QDialog>
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"

namespace Ui {
class ThresholdToolWindow;
}

class ThresholdToolWindow : public QDialog
{
    Q_OBJECT

public:
	ThresholdToolWindow(std::vector<bts::Patient> &patients);
    ~ThresholdToolWindow();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void on_comboBoxPatient_currentTextChanged(const QString &arg1);

    void on_checkBoxFindOptimal_stateChanged(int arg1);

private:
    Ui::ThresholdToolWindow *ui;

	void fillSlicesComboBox(bts::Patient patient);

	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatient;
	int processedDataStart = 0;

};

#endif // THRESHOLDTOOLWINDOW_H
