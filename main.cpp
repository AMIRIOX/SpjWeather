#include "SpjrSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SpjrSys w;
	w.setWindowTitle(u8"Special Judge����Ԥ��--Powered By Amiriox(�o��/������)");
	w.show();
	return a.exec();
}
