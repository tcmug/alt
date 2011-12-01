#ifndef ALTCONTEXT_H
#define ALTCONTEXT_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QStack>
#include <QScrollArea>

#include "altfilerow.h"

class AltFormatterSyntax;

class AltContext : public QWidget
{
	friend class AltFormatterBlockIterator;

  Q_OBJECT
  protected:

		QFont *Font;
		QFontMetrics *FontMetrics;

    void paintEvent(QPaintEvent *event);
		QVector <AltFileRow> Lines;
		QPoint CaretPosition;

		AltFormatterSyntax *Formatter;

  public:

    QSize minimumSizeHint() const;
	  QSize sizeHint() const;
				
    AltContext(QWidget *parent = 0);

	  void keyPressEvent(QKeyEvent *e);
		void openFile(const QString &fileName);
		QPoint insert(const QPoint &point, const QString &str);	
		QPoint erase(const QPoint &from, const QPoint &to);

	protected:

};

#endif
