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
		QString FileName;
		bool ShowFileName;

    void paintEvent(QPaintEvent *event);

		QVector <AltFileRow> Lines;
		QPoint CaretPosition;
		QPoint Selection[2];

		bool Painting;

		AltFormatterSyntax *Formatter;

		int getGutterWidth() const;
		void ensureCaretVisibility();

		bool hasSelection();

		void copy();
		void paste();

  	QPoint pointToCaretPosition(const QPoint &pt) const;
		
  public slots:
		
		void showFileName();
		void hideFileName();

  public:

		void resizeSelf();

    QSize minimumSizeHint() const;
	  QSize sizeHint() const;
				
    AltContext(QWidget *parent = 0);

		void openFile(const QString &file);
		void saveFile(const QString &file);
		
		QPoint insert(const QPoint &point, const QString &str);	
		QPoint erase(const QPoint &from, const QPoint &to);


		void mousePressEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);
		void mouseMoveEvent(QMouseEvent * event);

		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent(QKeyEvent *e);

};

#endif
