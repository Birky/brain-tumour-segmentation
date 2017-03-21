#define NOMINMAX // to avoid conflicts of macros: min and max
//#include <mutex>
/*#define INT64_MAX    _I64_MAX
#define INTMAX_MAX   INT64_MAX */// to avoid error in ratio.h
//#include "stdint.h"
//#include <cstdint>
#include "braintumoursegmentation.h"
#include "thresholdtoolwindow.h"
#include "resultwindow.h"
#include "imagearithmwindow.h"
#include "complexsegmentation.h"
#include "superpixelisationwindow.h"
#include <QFileDialog>
#include <QListView>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <SimpleITK.h>
#include <sitkMultiplyImageFilter.h>

/*#include <functional>
#include <thread>
#include <iostream>*/
//#include "CNTKLibrary.h"

//using namespace CNTK;

//#include "D:/FIIT/GIT/Source/Repos/gSLICr/gSLICr-master/gSLICr_Lib/gSLICr.h"

namespace sitk = itk::simple;

BrainTumourSegmentation::BrainTumourSegmentation(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
}

BrainTumourSegmentation::~BrainTumourSegmentation()
{

}

void BrainTumourSegmentation::on_actionOpen_image_s_triggered()
{
	// Multiple directory selection
	QFileDialog fd;
	fd.setFileMode(QFileDialog::DirectoryOnly);
	fd.setOption(QFileDialog::DontUseNativeDialog, true);
	fd.setDirectory("D:\\FIIT\\2-semester\\DP1\\Datasets\\BRATS2015_Training\\BRATS2015_Training\\HGG_16PNG\\");

	// get the list view of the fileDialog and set its selection mode to extended
	QListView *l = fd.findChild<QListView*>("listView");
	if (l) {
		l->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	QTreeView *t = fd.findChild<QTreeView*>();
	if (t) {
		t->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	if (fd.exec() != fd.Accepted)
	{
		return;
	}

	// get all png files from all selected dirs
	foreach(const QModelIndex &index, l->selectionModel()->selectedRows()) // loop patients
	{
		bts::Patient* patient = new bts::Patient();
		bts::MRData* inputData = new bts::MRData();
		std::vector<bts::Slice> slices[bts::modalityCount];
		patient->setOrginalData(inputData);

		// Get directory name and full path
		QString dirName = index.data(Qt::DisplayRole).toString();
		QString dirPath = fd.directory().absolutePath() + "/" + dirName;

		// fill patient by directory information
		patient->setPatientId(dirName.toStdString());
		patient->setDataFolder(dirPath.toStdString());

		std::string message = "Loading slices of patient \"" + dirName.toStdString() + "\".";
		ui.statusBar->showMessage(QString::fromStdString(message));

		QDir dir(dirPath);
		dir.setNameFilters(QStringList() << "*.png" << "*.PNG"); // filter out only the png image files
		

		// get all png files
		QFileInfoList fileInfoList = dir.entryInfoList();

		QString modality;
		foreach(const QFileInfo &fileInfo, fileInfoList) // loop slices of patient
		{
			QString filePath = fileInfo.absoluteFilePath();
			QString fileName = fileInfo.baseName();

			bts::Slice* slice = new bts::Slice(); // TODO spravit to cez konstruktor?
			// set the filePath
			slice->setFilePath(filePath.toStdString());

			// get modality
			modality = fileName.left(fileName.indexOf("_"));

			// get slice number
			int number = fileName.mid(fileName.indexOf("_") + 1, 3).toInt();
			slice->setNumber(number);

			// get slice data
			cv::Mat sliceData = cv::imread(filePath.toStdString(), CV_LOAD_IMAGE_ANYDEPTH);
			if (sliceData.data)
			{
				// set slice data
				slice->setData(sliceData);
			}

			// push_back the new slice
			slices[bts::modalityMap[modality.toStdString()]].push_back(*slice);
		}

		// set the slice count
		inputData->setSliceCount(slices[0].size());

		// TODO ak sa budes nudit spravi override setFunkciu ktora vlozi cele slices
		for (int i = 0; i < bts::modalityCount; i++)
		{
			inputData->setSlices(slices[i], i);
		}

		// calculate max intensities of all modalities
		inputData->calculateMaxIntensities();

		// store the patient into the list of patients
		patients.push_back(*patient);
		inputData->setPatient(&(patients.back()));
	}

	fillTreeWidget(patients);

	ui.statusBar->showMessage(QString::fromStdString("Loading data of patient(s) is done."));


	// TODO tu vyskúšaj CNTK model načítať
	/*std::string modelPath = "D:\\CNTK\\repos\\CNTK\\Examples\\Image\\GettingStarted\\OutputBRATS\\Models\\MLP_BRATS_3S_16epochs";

	std::wstring wstrModelPath(modelPath.begin(), modelPath.end());

	CNTK::Function::LoadModel(wstrModelPath, CNTK::DeviceDescriptor::GPUDevice(1));*/
}

void BrainTumourSegmentation::on_pushButton_clicked()
{

	cv::Mat img = cv::imread("E:\\FIIT_ing\\2-semester\\DP1\\Datasets\\BRATS2015_Training\\BRATS2015_Training\\HGG_16PNG\\pa0001\\Flair_068.png", CV_LOAD_IMAGE_ANYDEPTH);
	img.convertTo(img, CV_8UC1, 0.182);
	QImage imgIn = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);


	ui.labelTL->setPixmap(QPixmap::fromImage(imgIn));
	ui.labelTL->setScaledContents(true);
}

void BrainTumourSegmentation::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
	QTreeWidgetItem *selectedItem = ui.treeWidget->itemAt(pos);
	// context menu work only for leaf nodes
	if (selectedItem != nullptr && selectedItem->childCount() == 0)
	{
		// Create context menu
		/*QMenu mainMenu(this);
		QMenu addMenu(&mainMenu);

		// create the addMenu
		QPoint *pt = new QPoint(pos);
		QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TL"), this);
		newAct->setStatusTip(tr("Add to Top the Left window."));
		newAct->setObjectName("TL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		addMenu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TR"), this);
		newAct->setStatusTip(tr("Add to the Top Right window."));
		newAct->setObjectName("TR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		addMenu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BL"), this);
		newAct->setStatusTip(tr("Add to the Bottom Left window."));
		newAct->setObjectName("BL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		addMenu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BR"), this);
		newAct->setStatusTip(tr("Add to the Bottom Right window."));
		newAct->setObjectName("BR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		addMenu.addAction(newAct);

		// create mainMenu
		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Slices"), this);
		newAct->setObjectName("SLICES");
		newAct->setMenu(&addMenu);
		mainMenu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Evaluated slices"), this);
		newAct->setObjectName("ESLICES");
		newAct->setMenu(&addMenu);
		mainMenu.addAction(newAct);*/



		QMenu menu(this);

		// Create context menu actions
		// TODO z�skaj nejak� ikony a vylep�i si to
		QMenu subMenu(&menu);


		QPoint *pt = new QPoint(pos);
		QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TL"), this);
		newAct->setStatusTip(tr("Add to Top the Left window."));
		newAct->setObjectName("TL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TR"), this);
		newAct->setStatusTip(tr("Add to the Top Right window."));
		newAct->setObjectName("TR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BL"), this);
		newAct->setStatusTip(tr("Add to the Bottom Left window."));
		newAct->setObjectName("BL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BR"), this);
		newAct->setStatusTip(tr("Add to the Bottom Right window."));
		newAct->setObjectName("BR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		// For processed data add Evaluate
	/*	if (bts::isProcessedData(selectedItem->text(0).toStdString()))
		{
			newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Evaluate"), this);
			newAct->setStatusTip(tr("Evaluate the processed data."));
			newAct->setObjectName("Evaluate");
			newAct->setData(*pt);
			connect(newAct, SIGNAL(triggered()), this, SLOT(evaluate()));
			menu.addAction(newAct);
		}*/

		// exec context menu at the clicked position
		menu.exec(ui.treeWidget->mapToGlobal(pos));
	}
}

/*void BrainTumourSegmentation::evaluate()
{
	QAction *act = qobject_cast<QAction *>(sender());

	if (act != nullptr)
	{
		QPoint pos = act->data().toPoint();
		QTreeWidgetItem *selectedItem = ui.treeWidget->itemAt(pos);

		if (selectedItem != nullptr)
		{
			QString modality = selectedItem->text(0);
			// get patient
			QTreeWidgetItem *rootItem = selectedItem->parent();
			int patientIdx = bts::getPatientByName(patients, rootItem->text(0).toStdString());
			if (patientIdx != -1)
			{
				bts::Patient* patient = &(patients.at(patientIdx));
				bts::ProcessedData* processedData = patient->getProcessedData(modality.toStdString());

				std::vector<bts::Slice> slicesGT;
				slicesGT = patient->getOrginalData()->getSlices(bts::modalityMap["GT"]);

				processedData->evaluate(slicesGT);

			}

		}
	}
}
*/
void BrainTumourSegmentation::addToWindow()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act != nullptr)
	{
		QPoint pos = act->data().toPoint();
		QTreeWidgetItem *selectedItem = ui.treeWidget->itemAt(pos);

		if (selectedItem != nullptr)
		{
			QString modality = selectedItem->text(0);
			// get patient name
			QTreeWidgetItem *rootItem = selectedItem->parent();
			if (rootItem != nullptr)
			{
				QString patientName = rootItem->text(0);
				std::vector<bts::EvaluatedSlice> evaluatedSlices;
				int patientIdx = bts::getPatientByName(patients, patientName.toStdString());
				if (patientIdx != -1)
				{
					bts::Patient patient = patients.at(patientIdx);
					QSlider *slider;
					QLabel *label;
					QLabel *labelResults;

					float normalizationFactor;
					std::vector<bts::Slice> slices;

					if (bts::isProcessedData(modality.toStdString()))
					{
						if (ui.actionSegmented_Evaluated_data->isChecked())
						{
							evaluatedSlices = patient.getProcessedData(modality.toStdString())->getEvaluatedSlices();
							if (evaluatedSlices.size() > 0)
							{
								slices.assign(evaluatedSlices.begin(), evaluatedSlices.end());
							}
							else
							{
								slices = patient.getProcessedData(modality.toStdString())->getSlices();
							}

						}
						else
						{
							slices = patient.getProcessedData(modality.toStdString())->getSlices();
						}
						normalizationFactor = 1.0;

					}
					else
					{
						slices = patient.getOrginalData()->getSlices(bts::modalityMap[modality.toStdString()]);

						int maxIntensity;
						if (ui.actionGlobal_local_normalization->isChecked() || modality.compare("GT") == 0)
						{
							// local
							maxIntensity = patient.getOrginalData()->getIntensityMax(modality.toStdString());
						}
						else
						{
							// global
							maxIntensity = patient.getOrginalData()->getGlobalIntensityMax();
						}

						normalizationFactor = 1 / float(maxIntensity);
					}

					if (QString::compare(act->objectName(), "TL") == 0)
					{
						slider = ui.verticalSliderTL;
						label = ui.labelTL;
						slicesTL = slices;
						eSlicesTL = evaluatedSlices;
						nfTL = normalizationFactor;
						ui.labelTitleTL->setText(patientName + ": " + modality);
						labelResults = ui.labelResultsTL;
					}
					else if (QString::compare(act->objectName(), "TR") == 0)
					{
						slider = ui.verticalSliderTR;
						label = ui.labelTR;
						slicesTR = slices;
						eSlicesTR = evaluatedSlices;
						nfTR = normalizationFactor;
						ui.labelTitleTR->setText(patientName + ": " + modality);
						labelResults = ui.labelResultsTR;
					}
					else if (QString::compare(act->objectName(), "BL") == 0)
					{
						slider = ui.verticalSliderBL;
						label = ui.labelBL;
						slicesBL = slices;
						eSlicesBL = evaluatedSlices;
						nfBL = normalizationFactor;
						ui.labelTitleBL->setText(patientName + ": " + modality);
						labelResults = ui.labelResultsBL;
					}
					else if (QString::compare(act->objectName(), "BR") == 0)
					{
						slider = ui.verticalSliderBR;
						label = ui.labelBR;
						slicesBR = slices;
						eSlicesBR = evaluatedSlices;
						nfBR = normalizationFactor;
						ui.labelTitleBR->setText(patientName + ": " + modality);
						labelResults = ui.labelResultsBR;
					}
					labelResults->clear();

					int sliceCount = patient.getOrginalData()->getSliceCount();
					slider->setRange(1, sliceCount);
					slider->setValue(sliceCount / 2);

					// add initial slice into the selected window
					bts::Slice slice = slices.at(sliceCount / 2);
					cv::Mat img = slice.getData();

					if (evaluatedSlices.size() > 0)
					{
						bts::EvaluatedSlice eSlice = evaluatedSlices.at(sliceCount / 2);
						labelResults->setText("Dice: " + QString::number(eSlice.dice * 100.0, 'g', 2) + "% TP: " + QString::number(eSlice.TP) + " FP: " + QString::number(eSlice.FP) + " FN: " + QString::number(eSlice.FN));
					}

					QImage imgIn = convertMatToQImage(img, normalizationFactor);
					label->setPixmap(QPixmap::fromImage(imgIn));
					label->setScaledContents(true);
				}
			}
		}
	}
}

QImage BrainTumourSegmentation::convertMatToQImage(cv::Mat &img, float nf)
{
	switch (img.type())
	{
	case CV_16U:
	case CV_32F:
		img.convertTo(img, CV_8UC1, nf * 255);
	case CV_8UC1:
		return QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
	case CV_8UC3:
		return QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
	}
}


void BrainTumourSegmentation::on_verticalSliderTL_valueChanged(int value)
{
	if (slicesTL.size() != 0)
	{
		changeImage(&slicesTL, &eSlicesTL, ui.labelTL, ui.labelResultsTL, value, nfTL);
		// connect sliders
		if (ui.actionConnect_sliders->isChecked())
		{
			ui.verticalSliderBL->setValue(value);
			ui.verticalSliderBR->setValue(value);
			ui.verticalSliderTR->setValue(value);
		}
	}
	else
	{
		ui.verticalSliderTL->setValue(0);
	}
}

void BrainTumourSegmentation::on_verticalSliderBL_valueChanged(int value)
{
	if (slicesBL.size() != 0)
	{
		changeImage(&slicesBL, &eSlicesBL, ui.labelBL, ui.labelResultsBL, value, nfBL);
		// connect sliders
		if (ui.actionConnect_sliders->isChecked())
		{
			ui.verticalSliderBR->setValue(value);
			ui.verticalSliderTL->setValue(value);
			ui.verticalSliderTR->setValue(value);
		}
	}
	else
	{
		ui.verticalSliderBL->setValue(0);
	}
}

void BrainTumourSegmentation::on_verticalSliderTR_valueChanged(int value)
{
	if (slicesTR.size() != 0)
	{
		changeImage(&slicesTR, &eSlicesTR, ui.labelTR, ui.labelResultsTR, value, nfTR);
		// connect sliders
		if (ui.actionConnect_sliders->isChecked())
		{
			ui.verticalSliderBL->setValue(value);
			ui.verticalSliderTL->setValue(value);
			ui.verticalSliderBR->setValue(value);
		}
	}
	else
	{
		ui.verticalSliderTR->setValue(0);
	}
}

void BrainTumourSegmentation::on_verticalSliderBR_valueChanged(int value)
{
	if (slicesBR.size() != 0)
	{
		changeImage(&slicesBR, &eSlicesBR, ui.labelBR, ui.labelResultsBR, value, nfBR);

		// connect sliders
		if (ui.actionConnect_sliders->isChecked())
		{
			ui.verticalSliderBL->setValue(value);
			ui.verticalSliderTL->setValue(value);
			ui.verticalSliderTR->setValue(value);
		}
	}
	else
	{
		ui.verticalSliderBR->setValue(0);
	}
}


void BrainTumourSegmentation::changeImage(std::vector<bts::Slice> *slices, std::vector<bts::EvaluatedSlice> *eSlices, QLabel *labelImage, QLabel *labelResults, int value, float normalizationFactor)
{
	// change results
	if (eSlices->size() > 0)
	{
		bts::EvaluatedSlice eSlice = eSlices->at(value - 1);
		labelResults->setText("Dice: " + QString::number(eSlice.dice * 100.0, 'g', 4) + "% TP: " + QString::number(eSlice.TP) + " FP: " + QString::number(eSlice.FP) + " FN: " + QString::number(eSlice.FN));
		labelResults->repaint();
		labelResults->repaint();
	}
	
	// change image
	cv::Mat img = slices->at(value - 1).getData();

	QImage imgIn = convertMatToQImage(img, normalizationFactor);

	labelImage->setPixmap(QPixmap::fromImage(imgIn));
	labelImage->setScaledContents(true);

	ui.statusBar->showMessage(QString::number(value));
}

void BrainTumourSegmentation::on_actionThreshold_triggered()
{
	ThresholdToolWindow thresholdWin(patients);
	thresholdWin.setModal(this);
	thresholdWin.exec();

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::fillTreeWidget(std::vector<bts::Patient> patients)
{
	ui.treeWidget->clear();
	for each(bts::Patient patient in patients)
	{
		// Add patient node
		QTreeWidgetItem *treePatient = new QTreeWidgetItem(ui.treeWidget);
		treePatient->setText(0, QString::fromStdString(patient.getPatientId()));

		// Add modality nodes
		for (std::map<std::string, int>::iterator it = bts::modalityMap.begin(); it != bts::modalityMap.end(); ++it)
		{
			if (patient.getOrginalData()->getSlices(it->second).size() > 0)
			{
				QTreeWidgetItem *treeModality = new QTreeWidgetItem(treePatient);
				treeModality->setText(0, QString::fromStdString(it->first));
			}
		}

		// Add processed data nodes
		for each (bts::ProcessedData processedData in patient.getProcessedData())
		{
			QTreeWidgetItem *treeProcessedData = new QTreeWidgetItem(treePatient);
			treeProcessedData->setText(0, QString::fromStdString(processedData.getTitle()));
		}

	}
}
void BrainTumourSegmentation::on_actionShow_results_triggered()
{
	ResultWindow resultWin(patients);
	resultWin.setModal(false);
	resultWin.exec();
}

void BrainTumourSegmentation::on_actionImage_arithm_triggered()
{
	ImageArithmWindow imgArithmWin(patients);
	imgArithmWin.setModal(true);
	imgArithmWin.exec();

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionDo_the_segmentation_triggered()
{
	// TODO dočasné, neskôr spravit okno s vyberom pacienta a modality, iba orginalne data
	std::vector<bts::Slice> slices = patients.at(0).getOrginalData()->getSlices(bts::modalityMap["Flair"]);
	// TODO mozno by bolo dobre najprv spravit korekciu a robit to natom
	// TODO mozno aj to morfologické otvorenie

	float maxIntensity = patients.at(0).getOrginalData()->getGlobalIntensityMax();
	// Sprav threshold
	std::vector<bts::Slice> segmentedSlices = bts::doOptimalThreshold(slices, 10, 1.5, 50, 1 / (float)maxIntensity, true);

	// Z thresholdu vypočítaj centroid a iné
	long x = 0, y = 0, z = 0;
	int count = 0;
	float intensity = 0.0;

	for (int i = 0; i < segmentedSlices.size(); i++)
	{
		bts::Slice slice = segmentedSlices.at(i);
		bts::Slice orgSlice = slices.at(i);
		for (int j = 0; j < 240; j++)
		{
			for (int k = 0; k < 240; k++)
			{
				if (slice.getData().at<float>(j, k) == 1)
				{
					count++;
					x += k;
					y += j;
					z += i;
					intensity += orgSlice.getData().at<unsigned short>(j, k);
				}
			}
		}
	}
	x /= count;
	y /= count;
	z /= count;
	intensity /= (float)count;
	//intensity /= (float)maxIntensity;

	// Prejdi všetky voxely a vypočítaj špeciálnu vzdialenosť od centroidu
	std::vector<bts::Slice> processedSlices;
	for (int i = 0; i < segmentedSlices.size(); i++)
	{
		bts::Slice slice = slices.at(i);
		cv::Mat outputImage = slice.getData();
		outputImage.convertTo(outputImage, CV_32F);
		for (int j = 0; j < 240; j++)
		{
			for (int k = 0; k < 240; k++)
			{
				float xyzDif = sqrt(pow(x - k, 2) + pow(y - j, 2) + pow(z - i, 2));
				float intDif = (intensity - slice.getData().at<unsigned short>(j, k))/maxIntensity;
				float totalDif = xyzDif*2 + intDif*960;
				

				// Save results
				if (totalDif < 100)
				{
					outputImage.at<float>(j, k) = 1.0;
				}
else
{
	outputImage.at<float>(j, k) = 0.0;
}
			}
		}
		//cv::imshow(std::to_string(i), outputImage);
		slice.setData(outputImage);
		processedSlices.push_back(slice);
	}

	bts::ProcessedData *processedData = new bts::ProcessedData();
	processedData->setSlices(processedSlices);
	processedData->setPatient(&patients.at(0));
	processedData->setTitle("Segmentation");


	// Evaluate the new processed data
	std::vector<bts::Slice> slicesGT;
	slicesGT = patients.at(0).getOrginalData()->getSlices(bts::modalityMap["GT"]);
	processedData->evaluate(slicesGT);

	std::vector<bts::ProcessedData> pd = patients.at(0).getProcessedData();
	pd.push_back(*processedData);
	patients.at(0).setProcessedData(pd);

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionComplex_segmentation_triggered()
{
	ComplexSegmentation imgArithmWin(patients);
	imgArithmWin.setModal(true);
	imgArithmWin.exec();

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionOpen_mha_mhd_files_triggered()
{
	// Multiple directory selection
	QFileDialog fd;
	fd.setFileMode(QFileDialog::DirectoryOnly);
	fd.setOption(QFileDialog::DontUseNativeDialog, true);
	fd.setDirectory("D:\\FIIT\\2-semester\\DP1\\Datasets\\BRATS2015_Training\\BRATS2015_Training");

	// get the list view of the fileDialog and set its selection mode to extended
	QListView *l = fd.findChild<QListView*>("listView");
	if (l) {
		l->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	QTreeView *t = fd.findChild<QTreeView*>();
	if (t) {
		t->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	if (fd.exec() != fd.Accepted)
	{
		return;
	}

	int pbTotal = l->selectionModel()->selectedRows().size(), pbDone = 0;
	float pbState = 0;
	ui.progressBar->setValue(pbState);
	ui.progressBar->repaint();

	// get all mha files from all selected dirs
	foreach(const QModelIndex &index, l->selectionModel()->selectedRows()) // loop patients
	{
		// set up data structure for patient
		bts::Patient* patient = new bts::Patient();
		bts::MRData* inputData = new bts::MRData();
		std::vector<bts::Slice> slices[bts::modalityCount];
		patient->setOrginalData(inputData);

		// Get directory name and full path
		QString dirName = index.data(Qt::DisplayRole).toString();
		QString dirPath = fd.directory().absoluteFilePath(dirName);

		// fill patient by directory information
		patient->setPatientId(dirName.toStdString());
		patient->setDataFolder(dirPath.toStdString());

		std::string message = "Loading slices of patient \"" + dirName.toStdString() + "\".";
		ui.statusBar->showMessage(QString::fromStdString(message));

		QDir patientDir(dirPath);
		patientDir.setFilter(QDir::Dirs);

		QFileInfoList dirInfoList = patientDir.entryInfoList();
		dirInfoList.removeFirst(); // remove .
		dirInfoList.removeFirst(); // remove ..
		foreach(const QFileInfo &dirInfo, dirInfoList) // loop modalities
		{
			QDir modalityDir(dirInfo.absoluteFilePath());
			modalityDir.setNameFilters(QStringList() << "*.mha" << "*.mhd"); // filter out only the mha/mhd files

			// Get all mha/mhd files
			QFileInfoList fileInfoList = modalityDir.entryInfoList();
			foreach(const QFileInfo &fileInfo, fileInfoList) // loop files
			{
				// load mha/mhd file and save it to cv::Mat
				sitk::ImageFileReader reader;
				QString fileName = fileInfo.fileName();
				reader.SetFileName(fileInfo.absoluteFilePath().toStdString());
				sitk::Image mri = reader.Execute();

				// get modality
				QString modality = getModality(fileName);

				// get slices
				for (int i = 0; i < mri.GetDepth(); i++)
				{
					std::vector<unsigned int> size = mri.GetSize();
					size[2] = 1; // co-axial slicing, the depth(z) is set to 1
					std::vector<int> index(3, 0);
					index[2] = i;
					sitk::Image slice = sitk::RegionOfInterest(mri, size, index);

					if (slice.GetPixelIDValue() != sitk::sitkUInt16)
					{
						slice = sitk::Cast(slice, sitk::sitkUInt16);
					}

					bts::Slice* btsSlice = new bts::Slice();
					// set the filePath
					btsSlice->setFilePath(fileInfo.absoluteFilePath().toStdString());

					// get slice number
					btsSlice->setNumber(i);

					// get slice data
					cv::Mat ocvImage(slice.GetHeight(), slice.GetWidth(),
						CV_16U, (void*)slice.GetBufferAsUInt16());

					if (ocvImage.data)
					{
						// set slice data
						btsSlice->setData(ocvImage.clone());

						//TODO len docasne pre uloženie png
						/*if (i == 82 && modality.compare("Flair") == 0)
						{
						imwrite("D:\\FIIT\\2-semester\\DP1\\MATLAB\\pat24_flair83.png", ocvImage);
						}*/
					}

					// push_back the new slice
					slices[bts::modalityMap[modality.toStdString()]].push_back(*btsSlice);

					// set the slice count
					inputData->setSliceCount(mri.GetDepth());
				}
			}
		}
		// set modalities to MRIData
		for (int i = 0; i < bts::modalityCount; i++)
		{
			inputData->setSlices(slices[i], i);
		}

		// calculate max intensities of all modalities
		inputData->calculateMaxIntensities();

		// store the patient into the list of patients
		patients.push_back(*patient);
		inputData->setPatient(&(patients.back()));

		//********************************
		// After loading do the segmentation too and evaluate
		//*******************************
		//bts::doComplexSegmentation(&(patients.back()));
		//bts::doComplexSegmentation(patient);

		// Set the progress bar
		pbDone++;
		pbState = pbDone / (float)pbTotal;
		ui.progressBar->setValue(pbState * 100);
		ui.progressBar->repaint();

		// CLEAR BECAUSE NOT ENOUGH RAM
		//free(inputData);
		//free(patient);
		//patients.clear();
	}

	// Convert 3D volume to txt format of CNTK
	//std::vector<bool> sequences = { false, false, false, true };
	//convertMhaToCNTKtxt(patients, sequences); // T1, T1c, T2, FLAIR
	
	fillTreeWidget(patients);

	ui.statusBar->showMessage(QString::fromStdString("Loading data of patient(s) is done."));
}

QString BrainTumourSegmentation::getModality(QString fileName)
{
	QStringList possibleModalities = (QStringList() << "Flair" << "T1c" << "T1" << "T2");
	for (size_t i = 0; i < possibleModalities.size(); i++)
	{
		if (fileName.contains(possibleModalities.at(i)))
		{
			return possibleModalities.at(i);
		}
	}

	if (fileName.contains("OT"))
	{
		return "GT";
	}

	return "UNKNOWN";
}
void BrainTumourSegmentation::on_actionSuperpixels_triggered()
{
	SuperpixelisationWindow superWin(patients);
	superWin.setModal(true);
	superWin.exec();

	fillTreeWidget(patients);
}
