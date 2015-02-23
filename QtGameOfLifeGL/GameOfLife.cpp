/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
#include <gameoflife.h>

// Qt
#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QtOpenGL>

GameOfLife::GameOfLife(QWidget *parent) 
    : GameOfLifeGL(parent)
{
  startSimulation();
}

int GameOfLife::neighbours(int x, int y)
{
  int ncount = 0;
  // local index for 'f' are: ix, iy
  //       global coords are: cx, cy
  for(int ix=0; ix < 3; ++ix) {
    for(int iy=0; iy < 3; ++iy) {
      int cx = x - 1 + ix;
      int cy = y - 1 + iy;
      
      if(cx == x && cy == y) continue;      
      if(_field3d[cx][cy]) 
        ncount++;
    }
  }
  return ncount;
}

void GameOfLife::paintGL() 
{
  GameOfLifeGL::paintGL();
  unsigned int colorIndex = 0;
  
  for(int x=0; x<X_DIM; ++x) {
    for(int y=0; y<Y_DIM; ++y) {
      if(!_field3d[x][y]) continue;
      
      glColor3f(color(colorIndex).red(), 
                color(colorIndex).green(), 
                color(colorIndex).blue()
               );
      
      r(x, y);

      ++colorIndex;
      if(colorIndex >= 8)
        colorIndex = 0;
    }
  }
}

void GameOfLife::updateField() 
{
  if(isPaused())
  {
    return;
  }

  int _field3dLocal[X_DIM][Y_DIM];
  memmove(_field3dLocal, _field3d, sizeof(int)*X_DIM*Y_DIM);
  
  // http://rosettacode.org/wiki/Conway%27s_Game_of_Life/C
  for(int x=1; x < X_DIM-1; ++x) {
    for(int y=1; y < Y_DIM-1; ++y) {
      int ncount = neighbours(x, y);
      // Alive...
      if(_field3dLocal[x][y]) {
        if(ncount < 2 || ncount > 3) _field3dLocal[x][y] = 0;
        else                         _field3dLocal[x][y] = 1;
      } 
      // Dead...
      else {
        if(ncount == 3) _field3dLocal[x][y] = 1;
        else            _field3dLocal[x][y] = 0;
      }
    }
  }
  
  memmove(_field3d, _field3dLocal, sizeof(int)*X_DIM*Y_DIM);
}

void GameOfLife::startSimulation() 
{
  _figureIdentifier = 6;
  reset();
}

void GameOfLife::restartSimulation() 
{
  disableTimer();
  reset();
  QTimer *t = new QTimer();
  t->setSingleShot(true);
  QObject::connect(t, SIGNAL(timeout()), this, SLOT(enableTimer()));
  t->start(5000);
}

#define MAXFIGURES 8

void GameOfLife::toggleFigure() 
{
  ++_figureIdentifier;
  if(_figureIdentifier >= MAXFIGURES)
    _figureIdentifier = 0;
  restartSimulation();
}

void GameOfLife::reset() 
{
#define P(x, y) _field3d[x][y] = 1
  memset(_field3d, 0, sizeof(int)*X_DIM*Y_DIM);
  switch(_figureIdentifier) {
  case 0: 
    // Gleiter
    P(2, 1);
    P(3, 2);
    P(1, 3);
    P(2, 3);
    P(3, 3);
    break;
  case 1: 
    // Unruhe (oszillierend)
    P(52, 50);
    P(50, 51);
    P(51, 51);
    P(52, 52);
    P(53, 52);
    P(51, 53);
    break;
  case 2: 
    // Blinker (oszillierend)
    P(50, 50); P(51, 50); P(52, 50);
    break;
  case 3:
    // Kreuz mit Lock (statisch)
               P(50, 50);
    P(49, 51);            P(51, 51);
               P(50, 52);
    break;
  case 4:
    // Block, Kubus (statisch)
    P(50, 50); P(51, 50);
    P(50, 51); P(51, 51);
    break;
  case 5:
    // Bipole, Blinker
    P(51, 51); P(52, 51);
    P(51, 52); P(52, 52);
                          P(53, 53); P(54, 53);
                          P(53, 54); P(54, 54);
    break;
  case 6:
    // "leeres" Spielfeld
    P(51, 51); P(52, 51); P(53, 51);
    P(51, 52);            P(53, 52);
    P(51, 53);            P(53, 53);
    
    P(51, 55);            P(53, 55);
    P(51, 56);            P(53, 56);
    P(51, 57); P(52, 57); P(53, 57);
    break;
  case 7:
    // 42
#undef P
#define P(x, y) _field3d[x][100-y] = 1
    P(51, 51);            P(53, 51);     P(55, 51); P(56, 51); P(57, 51);
    P(51, 52);            P(53, 52);                           P(57, 52);
    P(51, 53); P(52, 53); P(53, 53);     P(55, 53); P(56, 53); P(57, 53);
                          P(53, 54);     P(55, 54);
                          P(53, 55);     P(55, 55); P(56, 55); P(57, 55);
    break;
  }
#undef P
}
