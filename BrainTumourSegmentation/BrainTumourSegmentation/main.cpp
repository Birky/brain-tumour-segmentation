#include "braintumoursegmentation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BrainTumourSegmentation* w = new BrainTumourSegmentation();
	w->show();
	a.setWindowIcon(QIcon(":/BrainTumourSegmentation/icons/brain.png"));
	return a.exec();
}
