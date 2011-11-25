#ifndef ALTCONTEXT_H
#define ALTCONTEXT_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QStack>
#include <QScrollArea>

class AltFileRow 
{
	private:
	  QString String;
		QStack <QString> Stack;
		
	public:

		AltFileRow(const QString &str = "");
	
		const QString &getString();
		void setString(const QString &str);
		void setStack(const QStack <QString> &stack);
		const QStack <QString> &getStack();



};


class AltContext : public QWidget
{
  Q_OBJECT
  protected:

		QFont *Font;
		QFontMetrics *FontMetrics;

    void paintEvent(QPaintEvent *event);
		QVector <AltFileRow> Lines;
		QPoint CaretPosition;

  public:
   QSize minimumSizeHint() const;
	      QSize sizeHint() const;
				
  AltContext(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent *e);

  protected:

};

#endif
