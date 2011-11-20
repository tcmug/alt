
#include <QtGui>
#include "window.h"
#include "altcontext.h"

Window::Window()
{
	altContextWidget = new AltContext();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(altContextWidget);
	
    setLayout(mainLayout);
    setWindowTitle(tr("Alt"));
}

void Window::keyPressEvent(QKeyEvent *e)
{
if (e->key() == Qt::Key_Escape)
    close();
else
    QWidget::keyPressEvent(e);
}


