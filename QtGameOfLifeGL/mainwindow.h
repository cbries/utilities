/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#pragma once

// Qt
#include <QMainWindow>

// forward-declaration
class QTimer;

namespace Ui {
class MainWindow;
}

#define CELLS_X 100
#define CELLS_Y 25

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private:
  Ui::MainWindow *ui;

  QTimer *glTimer;
};

#endif // __MAINWINDOW_H__
