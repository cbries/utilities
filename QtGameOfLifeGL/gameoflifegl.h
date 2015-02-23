/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
#ifndef __GAMEOFLIFEGL_H__
#define __GAMEOFLIFEGL_H__

#pragma once

#include <QTimer>
#include <QColor>
#include <QVector>
#include <QGLWidget>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>

// C++
#include <vector>

#define X_DIM 100
#define Y_DIM 100

//! Rotation view withing the OpenGL scene.
#define XROTMIN -45.0f
#define YROTMIN -45.0f

//! z-coordination on the start of this simulation
#define ZCOORD -45.0f

#define OBJECT_WIRECUBE 0
#define OBJECT_SOLIDCUBE 1
#define OBJECT_TEXTURECUBE 2
#define OBJECT_LASTID 3

class GameOfLifeGL : public QGLWidget
{
  Q_OBJECT
public:
  explicit GameOfLifeGL(QWidget *parent = 0);
  
private:
  int _objectId;
  
  float _xrot, _yrot;
  float _zcoord;
  
  //! Timer which will update our OpenGL scene.
  QTimer *_timer;
    
  //! Colors for OpenGL cubes
  QVector<QColor> _colors;
  
  bool _paused;
  
protected:
  bool isPaused() { return _paused; }
  
private:
  
  //! Enables/Disables light mode
  bool _light;
  
  //! Enables/Disables blending
  bool _blending;
  
  //! Shows help of keyboard and mouse functionalities.
  void _help();
  
  //! Toggles all preference states
  void _toggles();
  
  //! Draws a cube with a texture
  void _drawCubeTexture();
  
protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL(); 
  void keyPressEvent(QKeyEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);
  
  float xrot() const      { return _xrot;       }
  float yrot() const      { return _yrot;       }
  float zcoord() const    { return _zcoord;     }
  const QColor & color(int index) const;
    
  //! Renders one shape on a specific coordinate.
  virtual void r(const QVector<int> & vec);
  virtual void r(int x, int y);
  
  //! Calculates how many neighbours are around of one field.
  virtual int neighbours(const QVector<int> & v);
  virtual int neighbours(int x, int y);
  
  //! Updates our playing field and sets a new generation.
  virtual void updateField();
    
public slots:
  virtual void timeOutSlot();  
  //! Restarts this simulation
  virtual void restartSimulation();
  //! Toggles the figure for this simulation
  virtual void toggleFigure();
  
  void disableTimer();
  void enableTimer();
};

#endif // __GAMEOFLIFEGL_H__
