
#include <QtGui>
#include "window.h"
#include "altcontext.h"

Window::Window()
{
	altContextWidget = new AltContext();
  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->setMargin(0);
	QScrollArea *scrollArea = new QScrollArea();
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(altContextWidget);
  mainLayout->addWidget(scrollArea);
  setLayout(mainLayout);
  setWindowTitle(tr("Alt"));
}

void Window::resizeEvent(QResizeEvent *e)
{
	altContextWidget->resizeSelf();
}


void Window::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Escape)
    close();
  else
    altContextWidget->keyPressEvent(e);
}


