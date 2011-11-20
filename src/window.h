#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class AltContext;

class Window : public QWidget 
{
  Q_OBJECT

  public:
    Window();

  protected:
    void keyPressEvent(QKeyEvent *event);

  private:
		
		AltContext *altContextWidget;


};

 #endif
