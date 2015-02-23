/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
// Qt
#include <QTimer>

#include <mainwindow.h>
#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  glTimer = new QTimer(this);
  QObject::connect( glTimer, SIGNAL(timeout()), ui->widget, SLOT(updateGL()));
  glTimer->start(10);
  
  showMaximized();
}

MainWindow::~MainWindow()
{
  delete ui;
}
