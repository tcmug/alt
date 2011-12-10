
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
#include "window.h"

void AltContext::keyPressEvent(QKeyEvent *e)
{ 
	int OldLines = Lines.size();

  switch (e->key()) {
	
		case Qt::Key_Up:
			if (CaretPosition.y() > 0) 
			{
				CaretPosition.setY(CaretPosition.y() - 1);
      	if (CaretPosition.x() > Lines[CaretPosition.y()].getString().length()) 
	      {
		      CaretPosition.setX(Lines[CaretPosition.y()].getString().length());
				}
			}
			repaint();
		break;
		case Qt::Key_Down:
			if (CaretPosition.y() < Lines.size() - 1) 
			{
				CaretPosition.setY(CaretPosition.y() + 1);
      	if (CaretPosition.x() > Lines[CaretPosition.y()].getString().length()) 
	      {
		      CaretPosition.setX(Lines[CaretPosition.y()].getString().length());
				}
			}
			repaint();
		break;
		case Qt::Key_Left:
		  if (CaretPosition.x() == 0) 
		  {
				if (CaretPosition.y() > 0) 
				{
	    	  CaretPosition.setY(CaretPosition.y() - 1);
  	 	    CaretPosition.setX(Lines[CaretPosition.y()].getString().length());
				}
		  } 
			else
			{
				CaretPosition.setX(CaretPosition.x() - 1);
			}
			repaint();
		break;
		case Qt::Key_Right:
			if (CaretPosition.x() == Lines[CaretPosition.y()].getString().length())
			{
				if (CaretPosition.y() < Lines.size() - 1)
				{
  				CaretPosition.setX(0);
					CaretPosition.setY(CaretPosition.y() + 1);
				}
			}
			else 
			{
				CaretPosition.setX(CaretPosition.x() + 1);
			}
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
			{
			QPoint From = CaretPosition + QPoint(-1, 0);
			if (From.x() < 0) 
			{
				if (From.y() == 0)
				  // do nothing as were on the 1st line
  				break;
				From.setY(From.y() - 1);
				From.setX(Lines[From.y()].getString().length());
			}
			CaretPosition = this->erase(From, CaretPosition);
			repaint();
		  }
		break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			CaretPosition = this->insert(CaretPosition, "\n");
			repaint();
		break;
		case Qt::Key_F2:
			{
			CaretPosition = this->insert(CaretPosition, "This\nis\na multiline insert");
			repaint();
		  }
		break;
		default:
			{
			//printf("%i\n",e->key());
			CaretPosition = this->insert(CaretPosition, e->text());
			repaint();
			}
	}

	if (OldLines != Lines.size()) 
	{
		resizeSelf();
	}

	this->ensureCaretVisibility();
}


void AltContext::resizeSelf() 
{
  // FIXME: replace 1000 with widest line in the file
	resize(1000, FontMetrics->height() * (Lines.size()) + 5);
}



QPoint AltContext::insert(const QPoint &p, const QString &str)
{
	QString temp;
	QPoint point = p;
	QStringList List = str.split("\n");
	int i;
	if (List.size() > 1) 
	{
		Lines.insert(point.y() + 1, List.size() - 1, AltFileRow());
		temp = Lines[point.y()].getString();
		if (temp.length() > point.x()) 
		{
			Lines[point.y() + List.size() - 1].setString(temp.right(temp.length() - point.x()));
			Lines[point.y()].setString(temp.left(point.x()));
		}
	  for (i = 0; i < List.size()-1; ++i) 
    {
			temp = List[i];
		  Lines[point.y()].insert(point.x(), temp);
		  point.setX(0);
		  point.setY(point.y() + 1);
	  }
		temp = List[i];
	} 
	else 
	{
		temp = str;
	}	

	Lines[point.y()].insert(point.x(), temp);
	point.setX(point.x() + temp.length());

	return point;
}


QPoint AltContext::erase(const QPoint &fromp, const QPoint &top)
{
	QPoint from = fromp;
	QPoint to = top;

	if (to.y() - from.y() > 1) 
	{
		// erase lines between
		Lines.remove(to.y() + 1, to.y() - from.y() - 1);
		to.setY(from.y() + 1);
	}

	if (to.y() - from.y() == 1) 
	{
		// on separate lines
		Lines[from.y()].erase(from.x(), Lines[from.y()].getString().length());
		Lines[to.y()].erase(0, to.x());
		Lines[from.y()].setString(
			Lines[from.y()].getString() +	Lines[to.y()].getString()
		);
		Lines.remove(to.y(), 1);
	} 
	else 
	{
		// on the same line
		Lines[from.y()].erase(from.x(), to.x());
	}

	return from;
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
	resizeSelf();
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
	setFocusPolicy(Qt::StrongFocus);

	Formatter = new AltFormatterSyntax();
	Lines.push_back(AltFileRow(""));
}


void AltContext::ensureCaretVisibility()
{
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

      if ((Line == CaretPosition.y()) &&
          (CaretPosition.x() >= Column) &&
          (CaretPosition.x() <= Column + bi->getPart().length()))
      {

        QPoint temp(
					Point.x() + FontMetrics->width(bi->getPart(), CaretPosition.x() - Column),
          Point.y()
        );
         
			  ((QScrollArea*)parentWidget()->parentWidget())->ensureVisible(temp.x(), temp.y());				
				//printf("%i %i\n", temp.x(), temp.y());
				repaint();
        return;
      }
    }

    Point += QPoint(FontMetrics->width(bi->getPart()), 0);
  }

}


void AltContext::mousePressEvent(QMouseEvent *event)
{
	int Line = event->y() / FontMetrics->height();
	CaretPosition.setY(Line);
	
	AltFormatterBlockIterator *bi = new AltFormatterBlockIterator(this);
	bi->setRow(Line);
	int x = 0, w, c = 0;
	while (bi->next()) 
	{
		if (Line != bi->getRow())
		  break;

		const QString &temp = bi->getPart();
		w = FontMetrics->width(temp);
		if (event->x() < x + w) 
		{			
			for (int i = 0; i < temp.length(); i++)
			{
				if (event->x() < x) 
				{
					CaretPosition.setX(c + i - 1);
					repaint();
					return;
				}
				x += FontMetrics->width(temp[i]);
			}
		}
		c += temp.length();
		x += w;
		//printf("%s\n", bi->getPart().toStdString().c_str());
	}
	CaretPosition.setX(Lines[CaretPosition.y()].getString().length());
	repaint();
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

