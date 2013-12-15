#include "viewer.h"

#include <QDebug>

using namespace std;

Viewer::Viewer() : m_massViewer(this->camera()){
    connect(&m_massViewer, &Mass_spring_viewer::newInfo, this, &Viewer::handleNewInfo);
    show_axis = false;
}

// Draws a spiral
vec2 Viewer::pick(int _x, int _y)
{
    // query current OpenGL viewing settings
    GLdouble projection[16], modelview[16];
    GLint    viewport[4];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // gluUnproject gives us the point on the near plane
    GLdouble wx=_x, wy=this->height() - _y, ox, oy, oz;
    gluUnProject(wx, wy, 0.0,
                 modelview,
                 projection,
                 viewport,
                 &ox, &oy, &oz );

    return vec2(ox, oy);
}

void Viewer::draw()
{
  const float nbSteps = 200.0;
  //  startScreenCoordinatesSystem(true);
  int coeff = 10;
    m_skybox.SkyBox_Draw(0, 0, 0, coeff, coeff, coeff);
  //  stopScreenCoordinatesSystem();
  m_massViewer.draw();

}

void Viewer::init()
{
  // Restore previous viewer state.
//  restoreStateFromFile();
  setAxisIsDrawn(false);

  //display color
  setForegroundColor(QColor::fromHsv(120, 255, 190));

  //init mouse
  setMouseBinding(Qt::RightButton, CAMERA, ROTATE);
  setMouseBinding(Qt::LeftButton, SELECT);

  m_time_step = 0.00015625;//0.01;
  this->setAnimationPeriod(2);
  startAnimation();

  //init camera
  camera()->setPosition(qglviewer::Vec(0.0f, 2.0f, 3.0f));
  camera()->setUpVector(qglviewer::Vec(0.0f, 1.0f, 0.0f));
  camera()->lookAt(qglviewer::Vec(0.0f, 0.0f, 0.0f));
  camera()->setSceneRadius(10);

//  camera()->setZNearCoefficient(0.0f);
//  camera()->setZClippingCoefficient(10.f);
//  setSceneBoundingBox(0.1, 1000);

  setMouseTracking(true);

  //manipulated frame
  setMouseBinding(Qt::LeftButton, SELECT);

  setManipulatedFrame( camera()->frame() );

    //skybox
  // Enable GL textures
//  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  m_skybox.SkyBox_CreateTexture();

    // Opens help window
    //  help();
}

QString Viewer::helpString() const
{
  QString text("<h2>Cloth Simulation</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
    bool overridden = m_massViewer.keyboard(e);
    if (!overridden) {
        switch (e->key())
        {
            // toggle automatic animation on/off
            case Qt::Key_Space:
            {
            if (animationIsStarted()) {
                    stopAnimation();
                } else {
                    startAnimation();
                }
                break;
            }
            // adjust time step
            case Qt::Key_T:
            {
                m_time_step *= (e->modifiers() == Qt::ShiftModifier ? 2.0 : 0.5);
                std::cout << "timestep : " << m_time_step << std::endl;
                handleNewInfo(QString("Timestep : %1").arg(m_time_step));
                break;
            }
            // do a single time step
            case Qt::Key_S:
            {
                if (!animationIsStarted()) {
                    animate();
                }
                break;
            }
            // show axis
            case Qt::Key_P:
            {

                show_axis = !show_axis;
                if(show_axis) {
                    setAxisIsDrawn(true);
                    handleNewInfo("Show axis");
                } else {
                    setAxisIsDrawn(false);
                    handleNewInfo("Hide axis");
                }

                break;
            }
            // parent's job
            default:
            {
                QGLViewer::keyPressEvent(e);
                break;
            }
        }
    }
}

//void Viewer::mousePressEvent(QMouseEvent* e)
//{
//    if (e->button() == Qt::LeftButton) {
//        vec2 mpos = pick(e->pos().x(), e->pos().y());
//        m_massViewer.mouse(false, mpos.data()[0], mpos.data()[1]);
//    } else {
//        QGLViewer::mousePressEvent(e);
//    }
//}

//void Viewer::mouseReleaseEvent(QMouseEvent *e)
//{
//    if (e->button() == Qt::LeftButton) {
//        m_massViewer.mouse(true, e->pos().x(), e->pos().y());
//    } else {
//        QGLViewer::mouseReleaseEvent(e);
//    }
//}

void Viewer::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {
        QPoint p = e->pos();
//        vec2 mpos = pick(p.x(), p.y());
        vec2 mpos(p.x(), p.y());
        //original code
//        m_massViewer.motion(mpos.data()[0],mpos.data()[1]);
        //new implementation
        if (!m_massViewer.moveSelectedParticule(mpos)) {
            QGLViewer::mouseMoveEvent(e); //handle default if selected was -1
        }
    } else {
        QGLViewer::mouseMoveEvent(e);
    }
}

void Viewer::animate()
{
    m_massViewer.time_integration(m_time_step);
}

void Viewer::handleNewInfo(const QString &info)
{
    displayMessage(info, 5000);
}

void Viewer::postSelection(const QPoint& p)
{
    m_massViewer.setSelected(selectedName());
}

void Viewer::drawWithNames()
{
    m_massViewer.drawWithNames();
}
