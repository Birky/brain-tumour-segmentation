#define NOMINMAX // to avoid conflicts of macros: min and max
#include "braintumoursegmentation.h"
#include "thresholdtoolwindow.h"
#include "resultwindow.h"
#include "spxclassifier.h"
#include "imagearithmwindow.h"
#include "complexsegmentation.h"
#include "superpixelisationwindow.h"
#include <QFileDialog>
#include <QListView>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <SimpleITK.h>
#include <sitkMultiplyImageFilter.h>

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

	// check wether a folder was selected
	if (l->selectionModel()->selectedRows().count() == 0) //TODO solve folder selection from inside the folder
	{
		QMessageBox::warning(this, tr("Open image/s"),
			tr("No folder was selected!"),
			QMessageBox::Ok);
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

		// check wether there is png image files in folder
		if (fileInfoList.size() == 0)
		{
			QMessageBox::warning(this, tr("Open image/s"),
				tr("There is no .png or .PNG files in the selected folder!"),
				QMessageBox::Ok);
			continue;
		}

		QString modality;
		foreach(const QFileInfo &fileInfo, fileInfoList) // loop slices of patient
		{
			QString filePath = fileInfo.absoluteFilePath();
			QString fileName = fileInfo.baseName();

			bts::Slice* slice = new bts::Slice(); // TODO do a constructor to fill automatically the following stuff
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

		// TODO do an override set function which add every slice by one call
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

void BrainTumourSegmentation::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
	QTreeWidgetItem *selectedItem = ui.treeWidget->itemAt(pos);

	// context menu work only for leaf nodes
	if (selectedItem != nullptr && selectedItem->childCount() == 0)
	{
		QMenu menu(this);

		// Create context menu actions
		QMenu subMenu(&menu);

		QPoint *pt = new QPoint(pos);
		QAction *newAct = new QAction(QIcon(":/BrainTumourSegmentation/icons/TL.ico"), tr("&Add to TL"), this);
		newAct->setStatusTip(tr("Add to the Top Left window."));
		newAct->setObjectName("TL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/BrainTumourSegmentation/icons/TR.ico"), tr("&Add to TR"), this);
		newAct->setStatusTip(tr("Add to the Top Right window."));
		newAct->setObjectName("TR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/BrainTumourSegmentation/icons/BL.ico"), tr("&Add to BL"), this);
		newAct->setStatusTip(tr("Add to the Bottom Left window."));
		newAct->setObjectName("BL");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		newAct = new QAction(QIcon(":/BrainTumourSegmentation/icons/BR.ico"), tr("&Add to BR"), this);
		newAct->setStatusTip(tr("Add to the Bottom Right window."));
		newAct->setObjectName("BR");
		newAct->setData(*pt);
		connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindow()));
		menu.addAction(newAct);

		// exec context menu at the clicked position
		menu.exec(ui.treeWidget->mapToGlobal(pos));
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
					if (slider->value() == 0)
					{
						slider->setValue(sliceCount / 2);
					}
					slider->setRange(1, sliceCount);
										

					// add initial slice into the selected window
					bts::Slice slice = slices.at(slider->value());
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
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

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
		treePatient->setExpanded(true);

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
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

	ResultWindow resultWin(patients);
	resultWin.setModal(false);
	resultWin.exec();
}

void BrainTumourSegmentation::on_actionImage_arithm_triggered()
{
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

	ImageArithmWindow imgArithmWin(patients);
	imgArithmWin.setModal(true);
	imgArithmWin.exec();

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionDo_the_segmentation_triggered()
{
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

	// Temporary. Run the method 1 on all of the loaded patients
	for (int i = 0; i < patients.size(); i++)
	{
		bts::doComplexSegmentation(&(patients.at(i)));
	}

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionComplex_segmentation_triggered()
{
	ComplexSegmentation complexSegmentation(patients);
	complexSegmentation.setModal(true);
	complexSegmentation.exec();

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

			// check wether there is any .mha or .mhd files in folder
			if (fileInfoList.size() == 0)
			{
				QMessageBox::warning(this, tr("Open image/s"),
					tr("There is no .mha or .mhd files in the selected folder!"),
					QMessageBox::Ok);
				continue;
			}

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

		// Set the progress bar
		pbDone++;
		pbState = pbDone / (float)pbTotal;
		ui.progressBar->setValue(pbState * 100);
		ui.progressBar->repaint();
	}
	
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
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

	SuperpixelisationWindow superWin(patients);
	superWin.setModal(true);
	superWin.exec();

	fillTreeWidget(patients);
}

void BrainTumourSegmentation::on_actionSPX_classification_triggered()
{
	// Check whether any data is loaded
	if (patients.size() == 0)
	{
		QMessageBox::warning(this, tr("No data is loaded"),
			tr("Please load some data, otherwise you cannot use this functionality."),
			QMessageBox::Ok);
		return;
	}

	SPXClassifier spxClassifierWin(patients);
	spxClassifierWin.setModal(true);
	spxClassifierWin.exec();

	fillTreeWidget(patients);
}
