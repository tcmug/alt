
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


/**
 * Constructor
 */ 
AltContext::AltContext(QWidget *parent)
     :  QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

  Font = new QFont("Courier", 12, QFont::Normal);
	Font->setStyleHint(QFont::TypeWriter);
	FontMetrics = new QFontMetrics(*Font);
	setFocusPolicy(Qt::StrongFocus);

	Formatter = new AltFormatterSyntax();
	Lines.push_back(AltFileRow(""));
  
	Selection[0] = Selection[1] = QPoint(0, 0);

}


/**
 * Returns true if there is an active selection
 */
bool AltContext::hasSelection() {
  return Selection[0] != Selection[1];
}


/**
 * Display the filename in the editor
 */
void AltContext::showFileName() 
{
  this->ShowFileName = true;
  repaint();
}


/**
 * Hide the filename in the editor
 */
void AltContext::hideFileName() 
{
  this->ShowFileName = false;
	repaint();
}


/**
 * Event handler for keypresses
 */
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
		
		case 79:
		{ // O
  		const Qt::KeyboardModifiers modifiers = e->modifiers();
		  if (modifiers == Qt::ControlModifier) 
		  {
        FileName = QFileDialog::getOpenFileName(
  		    this, 
			  	tr("Open File"),
          "",
          tr("Files (*.*)")
			  ); 
				openFile(FileName);
			}
			else
			{
			  CaretPosition = this->insert(CaretPosition, e->text());
			  repaint();
			}	
		}
		break;
		
		case 83: 
		{ // S
  		const Qt::KeyboardModifiers modifiers = e->modifiers();
		  if (modifiers == Qt::ControlModifier) 
		  {
			  if (FileName == "")
				{
          FileName = QFileDialog::getSaveFileName(
				    this, 
				  	tr("Save File"),
            "",
            tr("Files (*.*)")
				  );
				} 
				this->saveFile(FileName);
			}
			else
			{
			  CaretPosition = this->insert(CaretPosition, e->text());
			  repaint();
			}	
		}
		break;

		case 67: // c
		{
      const Qt::KeyboardModifiers modifiers = e->modifiers();
	    if (modifiers == Qt::ControlModifier)
		  {
				copy();
			}
			else
			{
        CaretPosition = this->insert(CaretPosition, e->text());
        repaint();	
			}
		}
		break;

		case 86: // v
		{
      const Qt::KeyboardModifiers modifiers = e->modifiers();
	    if (modifiers == Qt::ControlModifier)
		  {
				paste();
			}
			else
			{
        CaretPosition = this->insert(CaretPosition, e->text());
        repaint();	
			}
		}
		break;

		default:
			{
			//printf("%u\n", e->key());
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


void AltContext::keyReleaseEvent(QKeyEvent *e) {
}


void AltContext::copy() 
{
  QClipboard *clipboard = QApplication::clipboard();
 	QMimeData mimeData;
	mimeData.setText(Lines[CaretPosition.y()].getString());
  clipboard->setMimeData(&mimeData);
}

void AltContext::paste() 
{
  const QClipboard *clipboard = QApplication::clipboard();
  const QMimeData *mimeData = clipboard->mimeData();
  if (mimeData->hasText()) {
		CaretPosition = this->insert(CaretPosition, mimeData->text());
    repaint();
       //setTextFormat(Qt::PlainText);
  }
}

void AltContext::resizeSelf() 
{
  // FIXME: replace 1000 with widest line in the file
	int height = (FontMetrics->height() * (Lines.size()) + 5);
	if (height < parentWidget()->height()) 
	{
	  height = parentWidget()->height();
	}
	resize(1000, height);
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


/**
 * saveFile saves the content to the given file
 */
void AltContext::saveFile(const QString &fileName) 
{
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QTextStream out(&file);
	QVector <AltFileRow>::const_iterator i;
	for (i = Lines.begin(); i != Lines.end(); i++)
	{
	  out << i->getString() << "\n";
  }
	file.close();
	QTimer::singleShot(5000, this, SLOT(hideFileName()));
	showFileName();
}


/**
 * openFile opens the given file to its context
 */
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
	QTimer::singleShot(5000, this, SLOT(hideFileName()));
	resizeSelf();
	showFileName();
	CaretPosition = QPoint(0, 0);
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
	int StartX = getGutterWidth();

  AltFormatterBlockIterator *bi = new AltFormatterBlockIterator(this);
  QPoint Point(StartX, -LineHeight);
  Point.setY(-LineHeight);

  while (bi->next()) {

    if (Line != bi->getRow())
    {
      Line = bi->getRow();
      Point = QPoint(StartX, Point.y() + LineHeight);
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
				repaint();
        return;
      }
    }

    Point += QPoint(FontMetrics->width(bi->getPart()), 0);
  }
}


QPoint AltContext::pointToCaretPosition(const QPoint &pt) const
{
	QPoint Result(-1, -1);
	int Line = 0;
	
	if (pt.y() > 0) 
	{
    Line = pt.y() / FontMetrics->height();
  }

	if (Line >= Lines.size()) 
	{
  	return Result;
	}
	
	Result.setY(Line);
	
	AltFormatterBlockIterator *bi = new AltFormatterBlockIterator(this);
	bi->setRow(Line);
	
	int x = getGutterWidth(), w, c = 0;
	
	while (bi->next()) 
	{
	  // check for end of line
		if (Line != bi->getRow())
		  break;

		const QString &temp = bi->getPart();
		w = FontMetrics->width(temp);
		if (pt.x() <= x + w) 
		{	
			for (int i = 0; i < temp.length()+1; i++)
			{
			  int tx = x + FontMetrics->width(temp, i);
				if (pt.x() < tx) 
				{
					Result.setX(c + i - 1);
					return Result;
				}
			}
		}
		c += temp.length();
		x += w;
	}

	Result.setX(Lines[Result.y()].getString().length());
	return Result;
}


void AltContext::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) 
  {
    QPoint Result = pointToCaretPosition(event->pos());
	  if (Result.x() != -1 && Result.y() != -1) 
	  {
	    CaretPosition = Result;
			Selection[0] = Selection[1] = CaretPosition;
	 	  Painting = true;
	    repaint();
	  }  
  }
}


void AltContext::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) 
	{
    QPoint Result = pointToCaretPosition(event->pos());
	  if (Result.x() != -1 && Result.y() != -1) 
	  {
	    CaretPosition = Result;
			Selection[1] = CaretPosition;
		  Painting = false;
	    repaint();
			printf("%u:%u -> %u:%u\n", Selection[0].x(), Selection[0].y(), Selection[1].x(), Selection[1].y());
	  }  
	}
}


void AltContext::mouseMoveEvent(QMouseEvent *event) 
{
  if (Painting) 
	{
    QPoint Result = pointToCaretPosition(event->pos());
	  if (Result.x() != -1 && Result.y() != -1 &&
		  CaretPosition != Result) 
		{
	    CaretPosition = Result;
	    repaint();
	  }  
	}
}


QSize AltContext::minimumSizeHint() const
{
 return QSize(100, Lines.size() * 20);
}

QSize AltContext::sizeHint() const
{
  return QSize(400, 200);
}


int AltContext::getGutterWidth() const
{
  return 10;
}


/**
 * Implementation of paintEvent()
 */
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
	int StartX = getGutterWidth();
	AltFormatterBlockIterator *bi = new AltFormatterBlockIterator(this);
	QPoint Point(StartX, -LineHeight);
	Point.setY(-LineHeight);

	while (bi->next()) {

		if (Line != bi->getRow())
		{
			Line = bi->getRow();
		  Point = QPoint(StartX, Point.y() + LineHeight);
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

				QString xstr, ystr;
				xstr.setNum(CaretPosition.x() + 1);
				ystr.setNum(CaretPosition.y() + 1);
				QString out = xstr + ":" + ystr;
				int w = FontMetrics->width(out);
				QPoint pt(parentWidget()->width() - (w +5 ), Point.y() + LineHeight);
				painter.drawText(pt, out);

		  	painter.setCompositionMode(old);
		  }
		  if (bi->endOfLine()) 
		  	// Store existing Stack
		  	this->Lines[Line].setStack(bi->getLineStack());
		}
    

		if (bi->endOfLine()) 
			// Store existing Stack
		 	this->Lines[Line].setStack(bi->getLineStack());

		Point += QPoint(FontMetrics->width(bi->getPart()), 0);
  }
  
  Point = QPoint(0, Point.y() + LineHeight + 5);
  painter.setPen(Qt::black);
  painter.setBrush(QBrush(Qt::black));
  QRect temp = QRect(
    Point.x(),
		Point.y(),
    width(),
    1
  );
  painter.drawRect(temp);
 
  if (ShowFileName) 
	{
  	painter.setBackground(QBrush(Qt::white));
    painter.setPen(Qt::lightGray);
  	int w = FontMetrics->width(FileName);
    QPoint pt(parentWidget()->width() - (w + 5 ), LineHeight - y());
    painter.drawText(pt, FileName);
		pt += QPoint(-1,-1);
		painter.setBackgroundMode(Qt::TransparentMode);
  	painter.setPen(Qt::black);
    painter.drawText(pt, FileName);
	}
  
}

