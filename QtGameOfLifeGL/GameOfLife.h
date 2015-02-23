/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */

#ifndef __GAMEOFLIFE_H__
#define __GAMEOFLIFE_H__

#pragma once

#include <gameoflifegl.h>

class GameOfLife : public GameOfLifeGL
{
  Q_OBJECT
private:
  //! Only positive values are allowed.
  int _field3d[X_DIM][Y_DIM];
  
  //! Counts all neighbours for one coordinate.
  int neighbours(int x, int y);
  
  //! Current figure for simulation
  int _figureIdentifier;
  
public:
  explicit GameOfLife(QWidget *parent = 0);
 
  void paintGL();
  void updateField();
  void startSimulation();
  void restartSimulation();
  void toggleFigure();
  
private:
  void reset();
};

#endif // __GAMEOFLIFE_H__
