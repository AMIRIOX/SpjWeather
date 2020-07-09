#include "SpjrSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SpjrSys w;
	w.setWindowTitle(u8"Special JudgeÌìÆøÔ¤±¨--Powered By Amiriox(ŸoêÔ/ÁõÃÏÑô)");
	w.show();
	return a.exec();
}
