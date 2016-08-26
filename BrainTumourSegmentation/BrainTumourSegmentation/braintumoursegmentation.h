#ifndef BRAINTUMOURSEGMENTATION_H
#define BRAINTUMOURSEGMENTATION_H

#include <QtWidgets/QMainWindow>
#include "ui_braintumoursegmentation.h"
#include "../BTS_toolbox/segfunc.h"
#include "../BTS_toolbox/Patient.h"

class BrainTumourSegmentation : public QMainWindow
{
	Q_OBJECT

public:
	BrainTumourSegmentation(QWidget *parent = 0);
	~BrainTumourSegmentation();

private slots:
    void on_actionOpen_image_s_triggered();

    void on_pushButton_clicked();

private:
	Ui::BrainTumourSegmentationClass ui;

public:
	std::vector<bts::Patient> patients;
};

#endif // BRAINTUMOURSEGMENTATION_H
