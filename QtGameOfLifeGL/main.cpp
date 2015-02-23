/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
#include <mainwindow.h>

// Qt
#include <QApplication>
#include <QtOpenGL>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  
  return a.exec();
}
