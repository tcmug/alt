
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
		case Qt::Key_F1: {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("Files (*.*)"));
			this->openFile(fileName);
			}
		break;
		case Qt::Key_Backspace:
		  // erase from file point - to file point
			//this->erase(CaretPosition);
		break;
		case Qt::Key_F2:
			{
			this->insert(CaretPosition, "This\nis\na multiline insert");
			repaint();
		  }
		break;
		default:
			{
			QString str = e->text();
			this->insert(CaretPosition, str);
			CaretPosition.setX(CaretPosition.x() + str.length());
			repaint();
			}
	}

	if (CaretPosition.x() < 0) 
	{
		CaretPosition.setX(0);
	}
	else if (CaretPosition.x() >= 1000) 
	{
		CaretPosition.setX(1000);
	}

	if (CaretPosition.y() < 0) 
	{
		CaretPosition.setY(0);
	}
	else if (CaretPosition.y() >= Lines.size()) 
	{
		CaretPosition.setY(Lines.size() - 1);
	}
	
/*} else {
	QWidget::keyPressEvent(e);
}*/
}


void AltContext::insert(const QPoint &pointx, const QString &str)
{
/*
this willbe split
->
this will this was
inserted
to the file be split

*/
	QPoint point = pointx;
	QString temp;
	QStringList List = str.split("\n");
	if (List.size() > 1) 
	{
		Lines.insert(point.y() + 1, List.size() - 1, AltFileRow());
		temp = Lines[point.y()].getString();
		if (temp.length() > point.x()) {
			Lines[point.y() + List.size() - 1].setString(temp.right(temp.length() - point.x()));
			Lines[point.y()].setString(temp.left(point.x()));
		}
	  for (int i = 0; i < List.size(); ++i) 
    {
			temp = List[i];
		  Lines[point.y()].insert(point.x(), temp);
		  point.setX(0);
		  point.setY(point.y() + 1);
	  }
		return;
	} 
	else 
	{
		temp = str;
	}
	
	Lines[point.y()].insert(point.x(), temp);
	point.setX(point.x() + temp.length());
	
}

void AltContext::openFile(const QString &fileName) 
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
 	Lines.clear();
  QTextStream in(&file);
	Lines.reserve(1000);
	QString line;
  while (!in.atEnd()) 
	{
    line = in.readLine();
    line = line.replace("\t", "  ");
    Lines.push_back(AltFileRow(line));
  }
	resize(500, FontMetrics->height() * (Lines.size()));
	repaint();
	CaretPosition = QPoint(0, 0);
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
	Lines.push_back(AltFileRow(""));
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

