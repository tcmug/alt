
#include <QApplication>
#include <QDesktopWidget>
//#include <QMessageBox>
#include "window.h"

#include "altformattersyntax.h"

int main(int argc, char *argv[])
{


	QApplication app(argc, argv);
/*
QMessageBox::information( 0,
      "Start Fullscreen?",
      "Would You Like To Run In Fullscreen Mode?",
      QMessageBox::Yes,
      QMessageBox::No | QMessageBox::Default );
*/
	Window window;
	window.resize(window.sizeHint());

	int desktopArea = QApplication::desktop()->width() *
					QApplication::desktop()->height();
	int widgetArea = window.width() * window.height();

	if (((float)widgetArea / (float)desktopArea) < 0.75f)
		window.show();
	else
		window.showMaximized();

	return app.exec();
}
