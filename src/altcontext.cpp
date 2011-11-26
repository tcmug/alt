
#include <QtGui>
#include <QRegExp>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>

#include "altcontext.h"
#include "altformattersyntax.h"
#include "altfilerow.h"
#include "altformatterblockiterator.h"


void AltContext::keyPressEvent(QKeyEvent *e)
{
  switch (e->key()) {
	
		case Qt::Key_Up:
			CaretPosition += QPoint(0, -1);
			repaint();
		break;
		case Qt::Key_Down:
			CaretPosition += QPoint(0, 1);
			repaint();
		break;
		case Qt::Key_Left:
			CaretPosition += QPoint(-1, 0);
			repaint();
		break;
		case Qt::Key_Right:
			CaretPosition += QPoint(1, 0);
			repaint();
		break;
		case Qt::Key_F1:
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("Files (*.*)"));

  QFile file(fileName);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
 		Lines.clear();
     QTextStream in(&file);
		 Lines.reserve(1000);
		 QString line;
     while (!in.atEnd()) {
       line = in.readLine();
			 line = line.replace("\t", "  ");
       Lines.push_back(AltFileRow(line));
    }
		resize(500, FontMetrics->height() * (Lines.size()));
		repaint();
		CaretPosition = QPoint(0, 0);
		break;
	}

	if (CaretPosition.x() < 0) {
		CaretPosition.setX(0);
	}
	if (CaretPosition.y() < 0) {
		CaretPosition.setY(0);
	}
/*} else {
	QWidget::keyPressEvent(e);
}*/
}

AltContext::AltContext(QWidget *parent)
     :  QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  Font = new QFont("Courier", 12, QFont::Normal);
	FontMetrics = new QFontMetrics(*Font);
	//setFocus();
  //setDisabled(false);
	setFocusPolicy(Qt::StrongFocus);

	Formatter = new AltFormatterSyntax();
}

QSize AltContext::minimumSizeHint() const
{
 return QSize(100, Lines.size() * 20);
}

QSize AltContext::sizeHint() const
{
  return QSize(400, 200);
}


void AltContext::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setFont(*Font);
	
	const AltFormatterBlock *b;

	QPoint point;
	QString output, str;

	//int wh = parentWidget()->height();
	int LineHeight = FontMetrics->height();
	int Line = -1;
	int Column = 0;
	int sy = y() - LineHeight;
	
	AltFormatterBlockIterator *bi = new AltFormatterBlockIterator(this);
	QPoint Point(0, -LineHeight);
	Point.setY(-LineHeight);

	while (bi->next()) {
	
		if (Line != bi->getRow())
		{
			Line = bi->getRow();
		  Point = QPoint(0, Point.y() + LineHeight);
			sy += LineHeight;
		}
		
		b = bi->getFormatterBlock();
		Column = bi->getColumn();
  	
		if (sy > -LineHeight) {

  		painter.setPen(b->getTextColor());
    	painter.setBackgroundMode(Qt::OpaqueMode);
    	painter.setBackground(QBrush(b->getBackgroundColor()));
  	  painter.drawText(Point + QPoint(0, LineHeight), bi->getPart());

      if ((Line == CaretPosition.y()) &&
          (CaretPosition.x() >= Column) &&
          (CaretPosition.x() <= Column + bi->getPart().length()))
	  	{

      	QPainter::CompositionMode old = painter.compositionMode();
	  		//	painter.setCompositionMode(QPainter::CompositionMode_Difference);
    		painter.setPen(Qt::black);
	  		painter.setBrush(QBrush(Qt::black));
      	QRect temp = QRect(
	  			Point.x() + FontMetrics->width(bi->getPart(), CaretPosition.x() - Column), 
		  		Point.y(),
		  		2,
		  		LineHeight
	  		);
	  		painter.drawRect(temp);
		  	painter.setCompositionMode(old);
		  }
		}
    
		Point += QPoint(FontMetrics->width(bi->getPart()), 0);
  }


}

