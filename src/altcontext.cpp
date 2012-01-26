
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

	LastUpdatedRow = 0;
	HistoryTop = 0;
	Selection[0] = Selection[1] = QPoint(0, 0);

  setCursor(Qt::IBeamCursor);
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
		
		// Leave these out
		case Qt::Key_Control:
		case Qt::Key_Shift:
		break;

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
			if (hasSelection()) 
			{
				eraseSelection();
			}
			else
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
			}
			repaint();
		}
		break;
		
		case Qt::Key_Enter:
		case Qt::Key_Return:
			CaretPosition = this->insert(CaretPosition, "\n");
			repaint();
		break;
		
		case Qt::Key_Z:
		{ // O
  		const Qt::KeyboardModifiers modifiers = e->modifiers();
		  if (modifiers & Qt::ControlModifier) 
			{
		  	if (modifiers & Qt::ShiftModifier) 
					redo();
  			else
					undo();
				repaint();
			}
			else
			{
			  CaretPosition = this->insert(CaretPosition, e->text());
			  repaint();
			}
		}
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
				if (hasSelection())
					eraseSelection();
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
				if (hasSelection())
					eraseSelection();
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
				if (hasSelection())
					eraseSelection();
        CaretPosition = this->insert(CaretPosition, e->text());
        repaint();	
			}
		}
		break;

		default:
			{
      const Qt::KeyboardModifiers modifiers = e->modifiers();
			if (modifiers != Qt::ControlModifier)
			{
			  if (hasSelection())
				  eraseSelection();
			}
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



void AltContext::keyReleaseEvent(QKeyEvent *e) 
{
}


/**
 * Undo according to history
 */
bool AltContext::undo()
{
	if (HistoryTop) 
	{
		HistoryTop--;
    switch (History[HistoryTop].Type)
	  {
	   	case AltHistoryEvent::Insert:
		  	CaretPosition = erase(History[HistoryTop].Positions[0], History[HistoryTop].Positions[1], false);	
		  break;
	 	  case AltHistoryEvent::Erase:
  	  	CaretPosition = insert(History[HistoryTop].Positions[0], History[HistoryTop].String, false);	
		  break;
    }
		return true;
	}
	return false;
}



/**
 * Redo according to history
 */
bool AltContext::redo()
{
	if (HistoryTop < History.size()) 
	{
    switch (History[HistoryTop].Type)
	  {
	   	case AltHistoryEvent::Erase:
		  	CaretPosition = erase(History[HistoryTop].Positions[0], History[HistoryTop].Positions[1], false);	
		  break;
	 	  case AltHistoryEvent::Insert:
  	  	CaretPosition = insert(History[HistoryTop].Positions[0], History[HistoryTop].String, false);
		  break;
    }
		HistoryTop++;
		return true;
	}
	return false;
}


/**
 * Grab text between points
 */
QString AltContext::grab(const QPoint &b, const QPoint &e) const 
{
	QString Copy;
	if (b.y() == e.y())
	{
		Copy = Lines[b.y()].getString().mid(
		  b.x(),
			e.x() - b.x()
		);
	}
	else
	{
		Copy = Lines[b.y()].getString().right(
			Lines[b.y()].getString().length() - b.x()
		) + "\n";
	  for (int Line = b.y() + 1; Line < e.y(); Line++)
	  {
			Copy += Lines[Line].getString() + "\n";
	  }
		Copy += Lines[e.y()].getString().left(e.x());
	}
	return Copy;
}

/**
 * Copy selected text
 */
void AltContext::copy() 
{
  QClipboard *clipboard = QApplication::clipboard();
 	QMimeData mimeData;
	QString Copy;

	if (hasSelection()) 
	{
	  int ss = 0, se = 1;
	  if ((Selection[1].y() < Selection[0].y()) ||
	  	((Selection[0].y() == Selection[1].y()) && (Selection[1].x() < Selection[0].x())) 
		  )
  	{
	  	se = 0;
		  ss = 1;
  	}
		Copy = grab(Selection[ss], Selection[se]);
	}
	else
	{
	  Copy = Lines[CaretPosition.y()].getString();
	}
  mimeData.setText(Copy);
  clipboard->setMimeData(&mimeData);
}


/**
 * Paste text from clipboard
 */
void AltContext::paste() 
{
  const QClipboard *clipboard = QApplication::clipboard();
  const QMimeData *mimeData = clipboard->mimeData();
  if (mimeData->hasText()) {
    if (hasSelection()) 
		{
			eraseSelection();
		}
		CaretPosition = insert(CaretPosition, mimeData->text());
    repaint();
  }
}

/**
 * Erase selection
 */
void AltContext::eraseSelection() 
{
	int ss = 0, se = 1;
	if (
		(Selection[1].y() < Selection[0].y()) ||
		((Selection[0].y() == Selection[1].y()) && (Selection[1].x() < Selection[0].x())) 
		)
	{
		se = 0;
		ss = 1;
	}
	CaretPosition = erase(Selection[ss], Selection[se]);
	Selection[0] = Selection[1];
}

/**
 * Resize the widget to fit the window and be the right height
 */
void AltContext::resizeSelf() 
{
  // FIXME: replace 1000 with widest line in the file
	int height = (getLineHeight() * (Lines.size()) + 5);
	if (height < parentWidget()->height()) 
	{
	  height = parentWidget()->height();
	}
	resize(1000, height);
}


/**
 * Insert text to Position
 */
QPoint AltContext::insert(const QPoint &p, const QString &str, bool record)
{
	AltHistoryEvent event;
	event.Type = AltHistoryEvent::Insert;
	event.Positions[0] = p;
	event.String = str;

	QString temp;
	QPoint point = p;
	if (str.indexOf("\n") >= 0) 
	{
  	int i = 0;
	  QStringList List = str.split("\n");
	  Lines.insert(point.y() + 1, List.size() - 1, AltFileRow());
		temp = Lines[point.y()].getString();
		if (temp.length() > point.x()) 
		{
			Lines[point.y()].setString(temp.left(point.x()));
		}
		temp = temp.right(temp.length() - point.x());
	  for (i = 0; i < List.size() - 1; i++) 
    {
		  Lines[point.y()].insert(point.x(), List[i]);
		  point.setX(0);
		  point.setY(point.y() + 1);
	  }
		Lines[point.y()] = temp;
	  temp = List[i];
	} 
	else 
	{
		temp = str;
	}	
	
	Lines[point.y()].insert(point.x(), temp);
	point.setX(point.x() + temp.length());

	event.Positions[1] = point;
	if (record) 
	{
		if (HistoryTop < History.size())
		{
			History.remove(HistoryTop, History.size() - HistoryTop);
		}
	  History.push_back(event);
	  HistoryTop++;
	}
	return point;
}



/**
 * Erase from position to a position
 */
QPoint AltContext::erase(const QPoint &fromp, const QPoint &top, bool record)
{

	AltHistoryEvent event;
  event.Type = AltHistoryEvent::Erase;
  event.Positions[0] = fromp;
  event.Positions[1] = top;
  event.String = grab(fromp, top);
	QPoint from = fromp;
	QPoint to = top;

	if (to.y() - from.y() > 1) 
	{
		// erase lines between
		Lines.remove(from.y() + 1, (to.y() - from.y()) - 1);
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

	if (record) 
	{
		if (HistoryTop < History.size())
		{
			History.remove(HistoryTop, History.size() - HistoryTop);
		}
	  History.push_back(event);
	  HistoryTop++;
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
 * Get the height of a single line
 */
int AltContext::getLineHeight() const
{
	return FontMetrics->height() + 1;
}


/**
 * openFile opens the given file to its context
 */
void AltContext::openFile(const QString &fileName) 
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
	LastUpdatedRow = 0;
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

/**
 * Ensure caret visibility
 */
void AltContext::ensureCaretVisibility()
{

  const AltFormatterBlock *b;

  QPoint point;
  QString output, str;

  //int wh = parentWidget()->height();
  int LineHeight = getLineHeight();
  int Line = -1;
  int Column = 0;
	int StartX = getGutterWidth();

  AltFormatterBlockIterator bi(this);
  QPoint Point(StartX, -LineHeight);
  Point.setY(-LineHeight);
 
	while (bi.next()) {

    if (Line != bi.getRow())
    {
      Line = bi.getRow();
      Point = QPoint(StartX, Point.y() + LineHeight);
    }

    b = bi.getFormatterBlock();
    Column = bi.getColumn();

    if (Point.y() > -LineHeight) {

      if ((Line == CaretPosition.y()) &&
          (CaretPosition.x() >= Column) &&
          (CaretPosition.x() <= Column + bi.getPart().length()))
      {

        QPoint temp(
					Point.x() + FontMetrics->width(bi.getPart(), CaretPosition.x() - Column),
          Point.y()
        );
         
			  ((QScrollArea*)parentWidget()->parentWidget())->ensureVisible(temp.x(), temp.y());				
				repaint();
        return;
      }

			if (bi.endOfLine()) 
		  	this->Lines[Line].setStack(bi.getLineStack());
		}
    
		if (bi.endOfLine()) 
   		this->Lines[Line].setStack(bi.getLineStack());
    
		Point += QPoint(FontMetrics->width(bi.getPart()), 0);
  }

}


/**
 * Transform point to a caret position
 */
QPoint AltContext::pointToCaretPosition(const QPoint &pt) const
{
	QPoint Result(-1, -1);
	int Line = 0;
	
	if (pt.y() > 0) 
	{
    Line = pt.y() / getLineHeight();
  }

	if (Line >= Lines.size()) 
	{
  	return Result;
	}
	
	Result.setY(Line);
	
	AltFormatterBlockIterator bi(this);
	bi.setRow(Line);
 	
	int x = getGutterWidth(), w, c = 0;
	
	while (bi.next()) 
	{
	  // check for end of line
		if (Line != bi.getRow())
		  break;

		const QString &temp = bi.getPart();
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


/**
 * Act on mouse click events
 */
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


/**
 * Acts on mouse release events
 */
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
	  }  
	}
}

/**
 * Act on mouse move event
 */
void AltContext::mouseMoveEvent(QMouseEvent *event) 
{
  if (Painting) 
	{
    QPoint Result = pointToCaretPosition(event->pos());
	  if (Result.x() != -1 && Result.y() != -1 &&
		  CaretPosition != Result) 
		{
	    CaretPosition = Result;
			Selection[1] = CaretPosition;
	    repaint();
	  }  
	}
}

/**
 * Get minimum size of the window
 */
QSize AltContext::minimumSizeHint() const
{
 return QSize(100, Lines.size() * 20);
}

/**
 * Get size hinting 
 */
QSize AltContext::sizeHint() const
{
  return QSize(400, 200);
}


/**
 * Get the width of the gutter
 */
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
	int LineHeight = getLineHeight();
	int Line = -1;
	int Column = 0;
	int StartX = getGutterWidth();

	AltFormatterBlockIterator bi(this);
	QPoint Point(StartX, -LineHeight);
	
	bool HasSelection = hasSelection();
	int ss = 0, se = 1;

	if (HasSelection && 
		(Selection[1].y() < Selection[0].y()) ||
		((Selection[0].y() == Selection[1].y()) && (Selection[1].x() < Selection[0].x())) 
		)
	{
		se = 0;
		ss = 1;
	}

  int FirstVisibleLine = y() / -LineHeight;
	int StartLine = 0;

	if (LastUpdatedRow < FirstVisibleLine) 
	{
  	StartLine = LastUpdatedRow;  
	}
	else 
	{
  	StartLine = FirstVisibleLine;  
	}

  bi.setRow(StartLine);
	Point.setY((StartLine * LineHeight) - LineHeight);
	
	while (bi.next() && Point.y() < parentWidget()->height() + -y()) {
		
		if (Line != bi.getRow())
		{
			Line = bi.getRow();
		  Point = QPoint(StartX, Point.y() + LineHeight);
		}
		b = bi.getFormatterBlock();
		Column = bi.getColumn();
  
   	painter.setBackgroundMode(Qt::OpaqueMode);

		if (HasSelection && Line >= Selection[ss].y() && Line <= Selection[se].y())
		{
		  QString Part = bi.getPart();
		  int s = 0, e = Part.length();

			if (Selection[ss].y() == Line) 
			{
			  if ((Selection[ss].x() >= Column) &&
					(Selection[ss].x() <= Column + e)) 
					s = Selection[ss].x() - Column;
				else if (Selection[ss].x() > Column + e)
					s = e;
			}

			if (Selection[se].y() == Line)
			{
        if ((Selection[se].x() >= Column) &&
          (Selection[se].x() <= Column + e))
          e = Selection[se].x() - Column;
				else if (Selection[se].x() < Column)
					e = s;
			}

			QPoint TempPoint = Point + QPoint(0, LineHeight);

			if (s < e) {
				if (s > 0)
				{
				  // print preceeding non-selection
			    painter.setPen(b->getTextColor());
          painter.setBackground(QBrush(b->getBackgroundColor()));
          painter.drawText(TempPoint, Part.left(s));
				  TempPoint += QPoint(FontMetrics->width(Part.left(s)), 0);
				}
					
				// print selection
			  painter.setPen(b->getBackgroundColor());
        painter.setBackground(QBrush(b->getTextColor()));
        painter.drawText(TempPoint, Part.mid(s, e - s));
				TempPoint += QPoint(FontMetrics->width(Part.mid(s, e - s)), 0);

				if (e < Part.length())
				{
				  // print proceeding non-selection
			    painter.setPen(b->getTextColor());
           painter.setBackground(QBrush(b->getBackgroundColor()));
           painter.drawText(TempPoint, Part.right(Part.length() - e));
				}
			} else {
			  painter.setPen(b->getTextColor());
        painter.setBackground(QBrush(b->getBackgroundColor()));
        painter.drawText(TempPoint, Part);
			}

		}
		else
	  {
		  // Not within selection
      painter.setPen(b->getTextColor());
      painter.setBackground(QBrush(b->getBackgroundColor()));
      painter.drawText(Point + QPoint(0, LineHeight), bi.getPart());
		}

    if ((Line == CaretPosition.y()) &&
        (CaretPosition.x() >= Column) &&
          (CaretPosition.x() <= Column + bi.getPart().length()))
	  {
     	QPainter::CompositionMode old = painter.compositionMode();
    	painter.setPen(Qt::black);
	  	painter.setBrush(QBrush(Qt::black));
     	QRect temp = QRect(
    		Point.x() + FontMetrics->width(bi.getPart(), CaretPosition.x() - Column), 
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
  		QPoint pt((-x()) + parentWidget()->width() - (w +5 ), Point.y() + LineHeight);

  	  painter.setBackground(QBrush(Qt::white));
      painter.setPen(Qt::lightGray);
      painter.drawText(pt, out);
	    pt += QPoint(-1,-1);
      painter.setBackgroundMode(Qt::TransparentMode);
      painter.setPen(Qt::black);
      painter.drawText(pt, out);

			//painter.drawText(pt, out);

		 	painter.setCompositionMode(old);
		}
		  
		if (bi.endOfLine()) 
   		this->Lines[Line].setStack(bi.getLineStack());

		Point += QPoint(FontMetrics->width(bi.getPart()), 0);
  }

	if (Line > LastUpdatedRow)
	{
  	LastUpdatedRow = Line;
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
    QPoint pt((-x()) + parentWidget()->width() - (w + 5 ), LineHeight - y());
    painter.drawText(pt, FileName);
		pt += QPoint(-1,-1);
		painter.setBackgroundMode(Qt::TransparentMode);
  	painter.setPen(Qt::black);
    painter.drawText(pt, FileName);
	}
  
}

