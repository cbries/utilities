#-------------------------------------------------
#
# Project created by QtCreator 2012-01-29T23:08:03
#
#-------------------------------------------------

QT += core gui opengl

INCLUDEPATH += .

TARGET = QtGameOfLifeGL
TEMPLATE = app

SOURCES += \
  main.cpp\
  mainwindow.cpp \
  GameOfLifeGL.cpp \
  OpenGLHelper.cpp \
  GameOfLife.cpp

HEADERS += \
  mainwindow.h \
  GameOfLifeGL.h \
  OpenGLHelper.h \
  GameOfLife.h

FORMS += mainwindow.ui

RESOURCES += \
    Images.qrc
