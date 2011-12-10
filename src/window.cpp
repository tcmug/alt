
#include <QtGui>
#include "window.h"
#include "altcontext.h"

Window::Window()
{
	AltContextWidget = new AltContext();
  QHBoxLayout *MainLayout = new QHBoxLayout();
  MainLayout->setMargin(0);
	ScrollArea = new QScrollArea();
  ScrollArea->setBackgroundRole(QPalette::Dark);
 	ScrollArea->setWidget(AltContextWidget);
  MainLayout->addWidget(ScrollArea);
  setLayout(MainLayout);
  setWindowTitle(tr("Alt"));
}


void Window::resizeEvent(QResizeEvent *e)
{
	AltContextWidget->resizeSelf();
}


void Window::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Escape)
    close();
  else
    AltContextWidget->keyPressEvent(e);
}



