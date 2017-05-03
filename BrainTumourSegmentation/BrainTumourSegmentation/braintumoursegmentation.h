#ifndef BRAINTUMOURSEGMENTATION_H
#define BRAINTUMOURSEGMENTATION_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
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

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

	void addToWindow();

    void on_verticalSliderTL_valueChanged(int value);

    void on_verticalSliderBL_valueChanged(int value);

    void on_verticalSliderTR_valueChanged(int value);

    void on_verticalSliderBR_valueChanged(int value);

    void on_actionThreshold_triggered();

    void on_actionShow_results_triggered();

    void on_actionImage_arithm_triggered();

    void on_actionDo_the_segmentation_triggered();

    void on_actionComplex_segmentation_triggered();

    void on_actionOpen_mha_mhd_files_triggered();

    void on_actionSuperpixels_triggered();

	void on_actionSPX_classification_triggered();

private:
	Ui::BrainTumourSegmentationClass ui;
	float nfTL = 1, nfTR = 1, nfBL = 1, nfBR = 1;
	std::vector<bts::Slice> slicesTL, slicesTR, slicesBL, slicesBR;
	std::vector<bts::EvaluatedSlice> eSlicesTL, eSlicesTR, eSlicesBL, eSlicesBR;

	void changeImage(std::vector<bts::Slice> *slices, std::vector<bts::EvaluatedSlice> *eSlices, QLabel *labelImage, QLabel *labelResults, int value, float normalizationFactor);

	void fillTreeWidget(std::vector<bts::Patient> patients);

	QImage convertMatToQImage(cv::Mat &img, float nf);

	QString getModality(QString fileName);

public:
	std::vector<bts::Patient> patients;
};

#endif // BRAINTUMOURSEGMENTATION_H
