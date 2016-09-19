#include "braintumoursegmentation.h"
#include "thresholdtoolwindow.h"
#include <QFileDialog>
#include <QListView>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
	fd.setDirectory("E:\\FIIT_ing\\2-semester\\DP1\\Datasets\\BRATS2015_Training\\BRATS2015_Training\\HGG_16PNG\\");

	// get the list view of the fileDialog and set its selection mode to extended
	QListView *l = fd.findChild<QListView*>("listView");
	if (l) {
		l->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
	fd.exec();

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
		QMenu menu(this);

		// Create context menu actions
		// TODO z�skaj nejak� ikony a vylep�i si to
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
		if (bts::isProcessedData(selectedItem->text(0).toStdString()))
		{
			newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Evaluate"), this);
			newAct->setStatusTip(tr("Evaluate the processed data."));
			newAct->setObjectName("Evaluate");
			newAct->setData(*pt);
			connect(newAct, SIGNAL(triggered()), this, SLOT(evaluate()));
			menu.addAction(newAct);
		}

		// exec context menu at the clicked position
		menu.exec(ui.treeWidget->mapToGlobal(pos));
	}
}

void BrainTumourSegmentation::evaluate()
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
				int patientIdx = bts::getPatientByName(patients, patientName.toStdString());
				if (patientIdx != -1)
				{
					bts::Patient patient = patients.at(patientIdx);
					QSlider *slider;
					QLabel *label;

					float normalizationFactor;
					std::vector<bts::Slice> slices;

					if (bts::isProcessedData(modality.toStdString()))
					{
						if (ui.actionSegmented_Evaluated_data->isChecked())
						{
							std::vector<bts::EvaluatedSlice> evaluatedSlices = patient.getProcessedData(modality.toStdString())->getEvaluatedSlices();
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
						nfTL = normalizationFactor;
					}
					else if (QString::compare(act->objectName(), "TR") == 0)
					{
						slider = ui.verticalSliderTR;
						label = ui.labelTR;
						slicesTR = slices;
						nfTR = normalizationFactor;
					}
					else if (QString::compare(act->objectName(), "BL") == 0)
					{
						slider = ui.verticalSliderBL;
						label = ui.labelBL;
						slicesBL = slices;
						nfBL = normalizationFactor;
					}
					else if (QString::compare(act->objectName(), "BR") == 0)
					{
						slider = ui.verticalSliderBR;
						label = ui.labelBR;
						slicesBR = slices;
						nfBR = normalizationFactor;
					}

					int sliceCount = patient.getOrginalData()->getSliceCount();
					slider->setRange(1, sliceCount);
					slider->setValue(sliceCount / 2);

					// add initial slice into the selected window
					cv::Mat img = slices.at(sliceCount / 2).getData();

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
		return QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
	case CV_8UC3:
		return QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
	}
}


void BrainTumourSegmentation::on_verticalSliderTL_valueChanged(int value)
{
	if (slicesTL.size() != 0)
	{
		changeImage(&slicesTL, ui.labelTL, value, nfTL);
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
		changeImage(&slicesBL, ui.labelBL, value, nfBL);
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
		changeImage(&slicesTR, ui.labelTR, value, nfTR);
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
		changeImage(&slicesBR, ui.labelBR, value, nfBR);

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


void BrainTumourSegmentation::changeImage(std::vector<bts::Slice> *slices, QLabel *label, int value, float normalizationFactor)
{
	cv::Mat img = slices->at(value - 1).getData();

	QImage imgIn = convertMatToQImage(img, normalizationFactor);
	//img.convertTo(img, CV_8UC1, normalizationFactor * 255);
	//QImage imgIn = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);

	label->setPixmap(QPixmap::fromImage(imgIn));
	label->setScaledContents(true);

	ui.statusBar->showMessage(QString::number(value));
}

void BrainTumourSegmentation::on_actionThreshold_triggered()
{
	ThresholdToolWindow thresholdWin(patients);
	thresholdWin.setModal(this);
	thresholdWin.exec();

	fillTreeWidget(patients);
	// TODO need to repaint the tree
	// prerob to tak ze spravis funkciu, ktora ti vykresli tree a nie rozhadzene v open trigger
	// a potom to tu zavolaj odovzdas mu patients a normalne to rovnako spravis nemalo by to byt problem
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