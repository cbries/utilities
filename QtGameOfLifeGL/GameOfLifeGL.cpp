/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
#include <gameoflifegl.h>

// Qt
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QTime>

#include <QtOpenGL>
#include <gl/GLU.h>
#include <gl/GL.h>

// Ries
#include <openglhelper.h>
  
/* white ambient light at half intensity (rgba) */
static GLfloat LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
/* super bright, full intensity diffuse light. */
static GLfloat LightDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
/* position of light (x, y, z, (position of light)) */
static GLfloat LightPosition[] = { 0.0f, 10.0f, 2.0f, 1.0f };

#define MAXTEXTURES 5
static QString filenames[] = {
  ":/images/box01.jpg",
  ":/images/box02.jpg",
  ":/images/box03.jpg",
  ":/images/box04.png",
  ":/images/box05.png"
};
static GLuint textures[MAXTEXTURES];
static GLint textureid = 0;

GameOfLifeGL::GameOfLifeGL(QWidget *parent) 
    : QGLWidget(parent)
    , _paused(false)
{
  setMouseTracking(true);
  setFocus();
  
  _help();
  
  _objectId = OBJECT_TEXTURECUBE;
  
  _xrot   = XROTMIN;
  _yrot   = YROTMIN;
  _zcoord = ZCOORD;
  
  _light      = true;
  _blending   = false;
  
  _colors.push_back(QColor(  0, 255, 255));  
  _colors.push_back(QColor(255,   0,   0));
  _colors.push_back(QColor(255, 255,   0));
  _colors.push_back(QColor(255,   0, 255));
  _colors.push_back(QColor(  0,   0, 255));  
  _colors.push_back(QColor(177, 200,  50));  
  _colors.push_back(QColor( 10, 200,  50));  
  _colors.push_back(QColor(117, 240,  90));  
           
  qsrand(QDateTime::currentDateTime().toTime_t());  

  _timer = new QTimer();
  connect((QObject*)_timer, SIGNAL(timeout()), (QObject*)this, SLOT(timeOutSlot()));
  _timer->start(100);
}

void GameOfLifeGL::timeOutSlot() 
{
  updateField();
}

void GameOfLifeGL::r(const QVector<int> & vec) 
{    
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, zcoord());
  glRotatef(xrot(), 1.0f, 0.0f, 0.0f);
  glRotatef(yrot(), 0.0f, 1.0f, 0.0f);
  
  glPushMatrix();
  glTranslatef(vec[0]+0.5f - X_DIM/2.0f, 
               vec[1]+0.5f - Y_DIM/2.0f, 
               0.0f
              );
  switch(_objectId) {
  case OBJECT_WIRECUBE: glutWireCube(1.0f); break;
  case OBJECT_SOLIDCUBE: glutSolidCube(1.0f); break;
  case OBJECT_TEXTURECUBE: {
    glColor3f( 1.0f, 1.0f, 1.0f );
    _drawCubeTexture(); 
  } break;
  }  
  glPopMatrix();
}

void GameOfLifeGL::r(int x, int y) 
{
  QVector<int> v(0);
  v.append(x);
  v.append(y);
  r(v);
}

int GameOfLifeGL::neighbours(const QVector<int> & v) 
{
  Q_UNUSED(v);
  qDebug() << "GameOfLifeGL::neighbours(): not implemented";  
  return 0;
}

int GameOfLifeGL::neighbours(int x, int y)
{
  QVector<int> v(0);
  v.append(x);
  v.append(y);
  r(v);
  
  return neighbours(v);
}

void GameOfLifeGL::updateField() 
{
  qDebug() << "GameOfLifeGL::updateField(): not implemented";
}

void GameOfLifeGL::initializeGL() 
{   
  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_POLYGON_SMOOTH);
  
  // set up light number 1.
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // add lighting. (ambient)
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // set light position.
  glEnable(GL_LIGHT1);
  glShadeModel(GL_SMOOTH);
  
  glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  _toggles();
  
  glGenTextures(MAXTEXTURES, textures);
  for( int i=0; i < MAXTEXTURES; i++ )
  {
    try 
    {
      QImage img;
      img.load(filenames[i]);
    
      QImage data;
      
      data = QGLWidget::convertToGLFormat( img );
                
      glBindTexture(GL_TEXTURE_2D, textures[i]);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);      
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      gluBuild2DMipmaps( GL_TEXTURE_2D, 4, data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, data.bits() );
    }
    catch( ... ) 
    {
      qDebug() << "Could not load file: " << filenames[i];
    }
  }
  glEnable(GL_TEXTURE_2D);
}

void GameOfLifeGL::resizeGL(int w, int h) 
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
   
  glLoadIdentity();
  gluPerspective(50.,((GLfloat)w)/((GLfloat)h),0.1f,1000.0f);
   
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GameOfLifeGL::paintGL() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}

void GameOfLifeGL::keyPressEvent(QKeyEvent *event) {
  switch(event->key()) {
  case Qt::Key_L:       _light     = !_light;     break;
  case Qt::Key_B:       _blending  = !_blending;  break;
  case Qt::Key_O: {
    _objectId++;
    if(_objectId >= OBJECT_LASTID) {
      _objectId = OBJECT_WIRECUBE;
    }
  } break;
  case Qt::Key_T: {
    textureid++;
    if(textureid >= MAXTEXTURES) {
      textureid = 0;
    }
  } break;
  case Qt::Key_R:       restartSimulation();   break;
  case Qt::Key_F:       toggleFigure();        break;
  case Qt::Key_Plus:    _zcoord += 1.0f;       break;
  case Qt::Key_Minus:   _zcoord -= 1.0f;       break;
  case Qt::Key_Escape:  QApplication::exit(0); break;
  case Qt::Key_Space:   _paused = !_paused;    break;
  default:
    event->ignore();
  break;
  }
  
  _toggles();
}

void GameOfLifeGL::wheelEvent(QWheelEvent *event) {
  _zcoord += (event->delta() / 8) / 15;
}

const QColor & GameOfLifeGL::color(int index) const {
  return _colors[index];
}

void GameOfLifeGL::mouseMoveEvent(QMouseEvent *event) {

  int x = (event->x() < 0 ? 0 : event->x());
  int y = (event->y() < 0 ? 0 : event->y());
  
  float rotX = y / (float) height();
  float rotY = x / (float) width();
  
  _xrot  = -45.0f;
  _xrot +=  90.0f * rotX;
  
  _yrot  = -45.0f;
  _yrot +=  90.0f * rotY;
}

void GameOfLifeGL::_help() {
  qDebug() << "Help";
  qDebug() << " ESC         Quit";
  qDebug() << " +/-         Zoom in/out within the OpenGL scene";
  qDebug() << " Mousewheel  Zoom in/out within the OpenGL scene";
  qDebug() << " l           Toggles lighting on/off";
  qDebug() << " b           Toggles blending on/off";
  qDebug() << " o           Changes objects for renderung";
  qDebug() << " r           Restarts the simulation and starts from beginning";
  qDebug() << " f           Toggles between different figures for this simulation";
}

void GameOfLifeGL::_toggles() {
  if(_light) {
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
  } else {
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);
  }
  
  if(_blending) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST); 
  } else {
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
  }
}

void GameOfLifeGL::_drawCubeTexture() 
{
  #define HWIDTH 0.5f
  #define P1() glVertex3f(-HWIDTH, HWIDTH, HWIDTH)
  #define P2() glVertex3f(-HWIDTH, -HWIDTH, HWIDTH)
  #define P3() glVertex3f(HWIDTH, -HWIDTH, HWIDTH)
  #define P4() glVertex3f(HWIDTH, HWIDTH, HWIDTH)
  #define P5() glVertex3f(-HWIDTH, HWIDTH, -HWIDTH)
  #define P6() glVertex3f(-HWIDTH, -HWIDTH, -HWIDTH)
  #define P7() glVertex3f(HWIDTH, -HWIDTH, -HWIDTH)
  #define P8() glVertex3f(HWIDTH, HWIDTH, -HWIDTH)
  
  /* Bild auf die Fläche setzen. */
  #define T1() glTexCoord2f(0.0f, 1.0f)
  #define T2() glTexCoord2f(0.0f, 0.0f)
  #define T3() glTexCoord2f(1.0f, 0.0f)
  #define T4() glTexCoord2f(1.0f, 1.0f)
  
  /* Textur laden und an alle weiteren Flächen binden. */
  glBindTexture( GL_TEXTURE_2D, textures[textureid] );
  
  glBegin( GL_QUADS );
    // front
    glNormal3f( 0.0f, 0.0f, 1.0f );
    T1(); P1();
    T2(); P2();
    T3(); P3();
    T4(); P4();
  
    // back
    glNormal3f( 0.0f, 0.0f, -1.0f );
    T1(); P8();
    T2(); P7();
    T3(); P6();
    T4(); P5();
  
    // left
    glNormal3f( -1.0f, 0.0f, 0.0f );
    T1(); P1();
    T2(); P5();
    T3(); P6();
    T4(); P2();
  
    // right
    glNormal3f( 1.0f, 0.0f, 0.0f );
    T1(); P4();
    T2(); P3();
    T3(); P7();
    T4(); P8();
  
    // bottom
    glNormal3f( 0.0f, -1.0f, 0.0f );
    T1(); P3();
    T2(); P2();
    T3(); P6();
    T4(); P7();
  
    // top
    glNormal3f( 0.0f, 1.0f, 0.0f );
    T1(); P5();
    T2(); P1();
    T3(); P4();
    T4(); P8();
  glEnd();
  #undef HWIDTH
}

void GameOfLifeGL::restartSimulation() {
  qDebug() << "GameOfLifeGL::restartSimulation(): not implemented";
}

void GameOfLifeGL::toggleFigure() {
  qDebug() << "GameOfLifeGL::toggleFigure(): not implemented";
}

void GameOfLifeGL::disableTimer() {
  _timer->stop();
}

void GameOfLifeGL::enableTimer() {
  _timer->setInterval(100);
  _timer->start();
}
