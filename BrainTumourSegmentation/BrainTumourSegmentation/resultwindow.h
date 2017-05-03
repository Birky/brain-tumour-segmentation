#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"


namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
	ResultWindow(std::vector<bts::Patient> &patients);
    ~ResultWindow();

private slots:
    void on_comboBoxPatient_currentIndexChanged(int index);

    void on_comboBoxEvaluatedData_currentIndexChanged(const QString &arg1);

private:
    Ui::ResultWindow *ui;

	void fillEvaluatedDataComboBox(bts::Patient patient);
	void fillTableWithResults();

	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatient;
};

#endif // RESULTWINDOW_H
