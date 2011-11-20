
#include <QtGui>
#include <QRegExp>
#include <QVector>
#include <QMap>

#include "altcontext.h"

 AltContext::AltContext(QWidget *parent)
     : QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);

/*
QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("Files (*.*)"));
*/
}

QSize AltContext::minimumSizeHint() const
{
 return QSize(100, 100);
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

  QFont f ("Courier", 12, QFont::Normal);
	painter.setFont(f);

  static AltFormatterSyntax *a = 0;
	if (!a) 
	{
	  a = new AltFormatterSyntax();
  }
	
	const AltFormatterBlock *b;

	QStack <QString> stack;
	stack.push("ROOT");

	//a->draw(this, "Hello world! <?php some php and some other crap", stack);
  // while lines to be drawn
  int at = 0;

  QString str = "Hello world! <?php some php and some other crap?> 1, 2, 3, 4";
	QString output;

	QFontMetrics fm(f);
	
	QVector <QString> lines;
	lines.push_back("Hello world");
	lines.push_back("This is <?php PHP CODE!");
	lines.push_back("which ends here ?>");
	lines.push_back("1, 2, 3, four, five, six, 7, 8");
	lines.push_back("last line");
	
	QVector <QString>::iterator i;
	for (i = lines.begin(); i != lines.end(); i++) {
		str = (*i);
		at = 0;
  	while (at < str.length()) 
  	{
  		b = a->getFormatterBlock(stack.top());
    	output = a->formatize(str, at, stack);
  		painter.setPen(b->getTextColor());
  		painter.setBackgroundMode(Qt::OpaqueMode);
  		painter.setBackground(QBrush(b->getBackgroundColor()));
  		painter.drawText(point, output);
    	point += QPoint(fm.width(output), 0);
  	}
		point = QPoint(0, point.y() + fm.height());
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



void AltContext::drawString(QPainter &painter, QString str, QPoint &at, QColor textcolor, QColor backgroundcolor)
{	
}





