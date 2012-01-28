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

class AltHistoryEvent 
{
	public:
	  enum EventType
		{
			Insert,
			Erase
		};

		EventType Type; 
		QPoint Positions[2];
		QString String;
};


class AltContext : public QWidget
{
	friend class AltFormatterBlockIterator;

  Q_OBJECT
  protected:

		QVector <AltHistoryEvent> History;
		int HistoryTop;

		QFont *Font;
		QFontMetrics *FontMetrics;
		int FontSize;
		QString FileName;
		bool ShowFileName;

		int LastUpdatedRow;
		
		void setFontSize(int size);
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
		bool undo();
		bool redo();
		void eraseSelection();
		
		
		QString grab(const QPoint &b, const QPoint &e) const;

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
		
		QPoint insert(const QPoint &point, const QString &str, bool record = true);	
		QPoint erase(const QPoint &from, const QPoint &to, bool record = true);


		void mousePressEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);
		void mouseMoveEvent(QMouseEvent * event);

		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent(QKeyEvent *e);

		int getLineHeight() const;

};

#endif
