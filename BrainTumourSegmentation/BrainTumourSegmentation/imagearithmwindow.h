#ifndef IMAGEARITHMWINDOW_H
#define IMAGEARITHMWINDOW_H

#include <QDialog>
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"

namespace Ui {
class ImageArithmWindow;
}

class ImageArithmWindow : public QDialog
{
    Q_OBJECT

public:
   // explicit ImageArithmWindow(QWidget *parent = 0);
	ImageArithmWindow(std::vector<bts::Patient> &patients);
    ~ImageArithmWindow();

private slots:
    void on_buttonBox_accepted();

    void on_patientAComboBox_currentTextChanged(const QString &arg1);

    void on_patientBComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::ImageArithmWindow *ui;

	void fillSlicesComboBox(bts::Patient patient, int image); // image 0 - A, 1 -B

	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatientA, *currentPatientB;
	int processedDataStart[2];
};

#endif // IMAGEARITHMWINDOW_H
