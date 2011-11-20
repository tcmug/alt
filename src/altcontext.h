#ifndef ALTCONTEXT_H
#define ALTCONTEXT_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class AltContext : public QWidget
{
  Q_OBJECT
  protected:
	  
    void paintEvent(QPaintEvent *event);
  public:
   QSize minimumSizeHint() const;
	      QSize sizeHint() const;
				
				AltContext(QWidget *parent = 0);

  protected:
			void drawString(QPainter &painter, QString str, QPoint &at, QColor textcolor, QColor backgroundcolor);


};

#endif
