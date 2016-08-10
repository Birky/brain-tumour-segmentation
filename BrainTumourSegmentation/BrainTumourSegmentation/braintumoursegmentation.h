#ifndef BRAINTUMOURSEGMENTATION_H
#define BRAINTUMOURSEGMENTATION_H

#include <QtWidgets/QMainWindow>
#include "ui_braintumoursegmentation.h"
#include "../BTS_toolbox/segfunc.h"

class BrainTumourSegmentation : public QMainWindow
{
	Q_OBJECT

public:
	BrainTumourSegmentation(QWidget *parent = 0);
	~BrainTumourSegmentation();

private slots:
    void on_actionOpen_image_s_triggered();

private:
	Ui::BrainTumourSegmentationClass ui;
};

#endif // BRAINTUMOURSEGMENTATION_H
