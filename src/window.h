#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class AltContext;
class QScrollArea;

class Window : public QWidget 
{
  Q_OBJECT

  public:
    Window();
	
		QScrollArea *ScrollArea;
		
  protected:
    void keyPressEvent(QKeyEvent *event);
		void resizeEvent(QResizeEvent *event);

  private:
		
		AltContext *AltContextWidget;


};

 #endif
