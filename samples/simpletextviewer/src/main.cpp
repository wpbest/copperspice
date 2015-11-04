#include <QtGui/QApplication>
#include <QtGui/QGtkStyle>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setStyle("plastique");
	MainWindow window;
	window.show();
	return app.exec();
}
