#include "braintumoursegmentation.h"
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
	foreach(const QModelIndex &index, l->selectionModel()->selectedRows())
	{
		bts::Patient* patient = new bts::Patient();
		bts::MRData* inputData = new bts::MRData();
		std::vector<bts::Slice> slices[bts::modalityCount];
		inputData->setPatient(patient); // TODO check if it is working well pointers

		// Get directory name and full path
		QString dirName = index.data(Qt::DisplayRole).toString();
		QString dirPath = fd.directory().absolutePath() + "/" + dirName;

		// fill patient by directory information
		patient->setPatientId(dirName.toStdString());
		patient->setDataFolder(dirPath.toStdString());

		// TODO toto nejak vyrieï¿½iï¿½ aby si vedel appendovaï¿½ na spravne miesto
		QTreeWidgetItem *treePatient = new QTreeWidgetItem(ui.treeWidget);
		treePatient->setText(0, dirName);

		std::string message = "Loading slices of patient \"" + dirName.toStdString() + "\".";
		ui.statusBar->showMessage(QString::fromStdString(message));

		QDir dir(dirPath);
		dir.setNameFilters(QStringList() << "*.png" << "*.PNG"); // filter out only the png image files
		
		// get all png files
		QFileInfoList fileInfoList = dir.entryInfoList();

		// TODO toto radsej tak ze sa spocitaju T1 subory, potom toto zmaz // calculate sliceCount
		//int sliceCount = fileInfoList.size() / bts::modalityCount;

		QString modality;
		foreach(const QFileInfo &fileInfo, fileInfoList)
		{
			QString filePath = fileInfo.absoluteFilePath();
			QString fileName = fileInfo.baseName();

			bts::Slice* slice = new bts::Slice(); // TODO spravit to cez konstruktor?
			// set the filePath
			slice->setFilePath(filePath.toStdString());

			// get modality
			QString newModality = fileName.left(fileName.indexOf("_"));
			if (QString::compare(modality, newModality))
			{
				modality = newModality;
				// create modality item to the tree
				QTreeWidgetItem *treeModality = new QTreeWidgetItem(treePatient);
				treeModality->setText(0, modality);
			}

			// get slice number
			int number = fileName.mid(fileName.indexOf("_") + 1, 3).toInt();
			slice->setNumber(number);

			// get slice data
			cv::Mat sliceData = cv::imread(filePath.toStdString(), CV_LOAD_IMAGE_ANYDEPTH);
			if (sliceData.data)
			{
				// set slice data
				slice->setDataOrg(sliceData);
			}

			// push_back the new slice
			slices[bts::modalityMap[modality.toStdString()]].push_back(*slice);
		}

		// set the slice count
		inputData->setSliceCount(slices[0].size());
	}

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
	// TODO
	QTreeWidget *tree = ui.treeWidget;

	QTreeWidgetItem *nd = tree->itemAt(pos);

	// Create context menu
	QMenu menu(this);

	// Create context menu actions
	// TODO získaj nejaké ikony a vylepši si to
	QAction *newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TL"), this);
	newAct->setStatusTip(tr("Add to Top the Left window."));
	connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindowTL())); // TODO vytvor funkciu kde to spracujes! POrozmyslaj ci jednu pre vsetky alebo pre kazdu zvlast
	menu.addAction(newAct);

	newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to TR"), this);
	newAct->setStatusTip(tr("Add to the Top Right window."));
	connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindowTL()));
	menu.addAction(newAct);

	newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BL"), this);
	newAct->setStatusTip(tr("Add to the Bottom Left window."));
	connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindowTL()));
	menu.addAction(newAct);

	newAct = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Add to BR"), this);
	newAct->setStatusTip(tr("Add to the Bottom Right window."));
	connect(newAct, SIGNAL(triggered()), this, SLOT(addToWindowTL()));
	menu.addAction(newAct);

	// exec context menu at the clicked position
	QPoint pt(pos);
	menu.exec(tree->mapToGlobal(pos));
}
