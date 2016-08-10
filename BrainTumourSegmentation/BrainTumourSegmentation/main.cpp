#include "braintumoursegmentation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BrainTumourSegmentation w;
	w.show();
	return a.exec();
}
