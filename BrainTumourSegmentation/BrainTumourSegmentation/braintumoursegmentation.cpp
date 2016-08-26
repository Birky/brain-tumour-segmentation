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
	foreach(const QModelIndex &index, l->selectionModel()->selectedIndexes())
	{
		bts::Patient* patient = new bts::Patient();

		QString dirName = index.data(Qt::DisplayRole).toString();
		QString dirPath = fd.directory().absolutePath() + "/" + dirName;
		QDir dir(dirPath);
		dir.setNameFilters(QStringList() << "*.png" << "*.PNG");
		QFileInfoList fileInfoList = dir.entryInfoList();
		foreach(const QFileInfo &fileInfo, fileInfoList)
		{
			QString filePath = fileInfo.absoluteFilePath();
			QString fileName = fileInfo.baseName();
		}
	}
}

void BrainTumourSegmentation::on_pushButton_clicked()
{
	
	cv::Mat img = cv::imread("E:\\FIIT_ing\\2-semester\\DP1\\Datasets\\BRATS2015_Training\\BRATS2015_Training\\HGG_16PNG\\pa0001\\Flair_068.png", CV_LOAD_IMAGE_ANYDEPTH);
	img.convertTo(img, CV_8UC1, 0.182);
	QImage imgIn = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
	

	ui.labelTL->setPixmap(QPixmap::fromImage(imgIn));
	ui.labelTL->setScaledContents(true);
}
