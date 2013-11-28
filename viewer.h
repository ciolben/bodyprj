#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/qglviewer.h>
#include "Mass_spring_viewer.h"
#include "QGLViewer/config.h"

#include <QKeyEvent>

class Viewer : public QGLViewer {
private :
    Mass_spring_viewer m_massViewer;

    /// value of time-step
    float m_time_step;

    vec2 pick(int x, int y);

protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

  virtual void keyPressEvent(QKeyEvent *e);
//  virtual void mousePressEvent(QMouseEvent *e);
//  virtual void mouseReleaseEvent(QMouseEvent* e);
  virtual void mouseMoveEvent(QMouseEvent* e);
  virtual void drawWithNames();
  virtual void postSelection(const QPoint& p);

  virtual void animate();

public slots:
    void handleNewInfo(const QString& info);

public :
    Viewer();
};

#endif // VIEWER_H
