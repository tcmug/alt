
#include <QtGui>
#include <QRegExp>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>

#include "altcontext.h"

AltFileRow::AltFileRow(const QString &str)
{
	String = str;
}

const QString &AltFileRow::getString()
{
	return String;
}


void AltFileRow::setString(const QString &str)
{
	String = str;	
}


void AltFileRow::setStack(const QStack <QString> &s)
{
	Stack = s;
}


const QStack <QString> &AltFileRow::getStack()
{
	return Stack;
}

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
		resize(500, FontMetrics->height() * (Lines.size() + 1));
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
}

QSize AltContext::minimumSizeHint() const
{
 return QSize(100, Lines.size() * 20);
}

QSize AltContext::sizeHint() const
{
  return QSize(400, 200);
}

#include "altformattersyntax.h"

void AltContext::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
  QPoint point(0, 20);

	painter.setFont(*Font);

  static AltFormatterSyntax *a = 0;
	if (!a) 
	{
	  a = new AltFormatterSyntax();
  }
	
	const AltFormatterBlock *b;

	QStack <QString> stack;
	stack.push("ROOT");

  int at = 0;

  QString str;
	QString output;

	QVector <AltFileRow>::iterator i;

	QPoint Scroll(0, 0);
	
	point = QPoint(Scroll.x(), Scroll.y() + FontMetrics->height());

	int sy = y();
	int wh = parentWidget()->height();
	bool draw;
	int LineNumber = 0;
	int lineHeight = FontMetrics->height();
	
	i = Lines.begin();
	
	while (sy < wh && i != Lines.end()) {
		str = (*i).getString();
		at = 0;
		draw = sy > -lineHeight;
  	while (at < str.length()) 
  	{
  		b = a->getFormatterBlock(stack.top());
    	output = a->formatize(str, at, stack);
			if (draw)
			{
  		  painter.setPen(b->getTextColor());
  		  painter.setBackgroundMode(Qt::OpaqueMode);
  		  painter.setBackground(QBrush(b->getBackgroundColor()));
  		  painter.drawText(point, output);
				if (LineNumber == CaretPosition.y())
				{	
					QPainter::CompositionMode old = painter.compositionMode();
					painter.setCompositionMode(QPainter::CompositionMode_Difference);
  		    painter.setPen(Qt::white);
					painter.setBrush(QBrush(Qt::white));
				  painter.drawRect(CaretPosition.x() * 10, sy, 20, lineHeight);
					painter.setCompositionMode(old);
				} 
			}
    	point += QPoint(FontMetrics->width(output), 0);
  	}
		(*i).setStack(stack);
		point = QPoint(Scroll.x(), point.y() + lineHeight);
		sy += lineHeight;
		LineNumber++;
		i++;
	}
}

class AltFormatter {
	protected:
		int State;
		QPainter Painter;
		QFontMetrics FontMetrics;
		QFont Font;
		QPoint At;

	public:
	/*
		AltFormatter(QPainter);
		void setFont(const QString &str);
		void setPosition(const QPoint &pos):
		void setTextColor(const QColor &color);
		void setBgColor(const QColor &color);
		void draw(QString &str);
*/

};







