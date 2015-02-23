/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */

#ifndef __OPENGLHELPER_H__
#define __OPENGLHELPER_H__

#pragma once

#include <gl/GL.h>

void drawCube();

void glutWireCube(GLdouble size);
void glutSolidCube(GLdouble size);

#endif // __OPENGLHELPER_H__
