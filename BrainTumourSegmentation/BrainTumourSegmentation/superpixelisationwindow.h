#ifndef SUPERPIXELISATIONWINDOW_H
#define SUPERPIXELISATIONWINDOW_H

#include <QDialog>
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"
#include <fstream>
#include "rapidxml.hpp"

namespace Ui {
class SuperpixelisationWindow;
}

class SuperpixelisationWindow : public QDialog
{
    Q_OBJECT

public:
    //explicit SuperpixelisationWindow(QWidget *parent = 0);
	SuperpixelisationWindow(std::vector<bts::Patient> &patients);
    ~SuperpixelisationWindow();

private slots:
    void on_radioButtonAll_clicked();

    void on_radioButtonSpecific_clicked();

    void on_checkBoxFeatures_stateChanged(int arg1);

    void on_buttonBox_accepted();

    void on_comboBoxPatient_currentTextChanged(const QString &arg1);

private:
    Ui::SuperpixelisationWindow *ui;

	void SuperpixelisationWindow::fillSlicesComboBox(bts::Patient patient);

	void SuperpixelisationWindow::superpixelisationSpecificData();
	void SuperpixelisationWindow::superpixelisationAllData();

	std::vector<bts::Patient> *patients;
	bts::Patient *currentPatient;
	int processedDataStart = 0;
};

#endif // SUPERPIXELISATIONWINDOW_H
