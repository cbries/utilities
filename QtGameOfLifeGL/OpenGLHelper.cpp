/* Copyright (C) Feb, 2012 Christian Benjamin Ries
 * Contact: mail@christianbenjaminries.de
 * Web: http://www.christianbenjaminries.de
 * Source: https://github.com/cbries
 * License: MIT
 */
 
// Qt
#include <QtOpenGL>
 
// 
#include <openglhelper.h>

void drawCube() 
{
  glBegin(GL_QUADS);		// Draw The Cube Using quads
    //glColor3f(0.0f,0.5f,0.0f);	// Color Blue
    glVertex3f( 0.5f, 0.5f,-0.5f);	// Top Right Of The Quad (Top)
    glVertex3f(-0.5f, 0.5f,-0.5f);	// Top Left Of The Quad (Top)
    glVertex3f(-0.5f, 0.5f, 0.5f);	// Bottom Left Of The Quad (Top)
    glVertex3f( 0.5f, 0.5f, 0.5f);	// Bottom Right Of The Quad (Top)
    //glColor3f(0.5f,0.5f,0.0f);	// Color Orange
    glVertex3f( 0.5f,-0.5f, 0.5f);	// Top Right Of The Quad (Bottom)
    glVertex3f(-0.5f,-0.5f, 0.5f);	// Top Left Of The Quad (Bottom)
    glVertex3f(-0.5f,-0.5f,-0.5f);	// Bottom Left Of The Quad (Bottom)
    glVertex3f( 0.5f,-0.5f,-0.5f);	// Bottom Right Of The Quad (Bottom)
    //glColor3f(0.5f,0.0f,0.0f);	// Color Red	
    glVertex3f( 0.5f, 0.5f, 0.5f);	// Top Right Of The Quad (Front)
    glVertex3f(-0.5f, 0.5f, 0.5f);	// Top Left Of The Quad (Front)
    glVertex3f(-0.5f,-0.5f, 0.5f);	// Bottom Left Of The Quad (Front)
    glVertex3f( 0.5f,-0.5f, 0.5f);	// Bottom Right Of The Quad (Front)
    //glColor3f(0.5f,0.5f,0.0f);	// Color Yellow
    glVertex3f( 0.5f,-0.5f,-0.5f);	// Top Right Of The Quad (Back)
    glVertex3f(-0.5f,-0.5f,-0.5f);	// Top Left Of The Quad (Back)
    glVertex3f(-0.5f, 0.5f,-0.5f);	// Bottom Left Of The Quad (Back)
    glVertex3f( 0.5f, 0.5f,-0.5f);	// Bottom Right Of The Quad (Back)
    //glColor3f(0.0f,0.0f,0.5f);	// Color Blue
    glVertex3f(-0.5f, 0.5f, 0.5f);	// Top Right Of The Quad (Left)
    glVertex3f(-0.5f, 0.5f,-0.5f);	// Top Left Of The Quad (Left)
    glVertex3f(-0.5f,-0.5f,-0.5f);	// Bottom Left Of The Quad (Left)
    glVertex3f(-0.5f,-0.5f, 0.5f);	// Bottom Right Of The Quad (Left)
    //glColor3f(0.5f,0.0f,0.5f);	// Color Violet
    glVertex3f( 0.5f, 0.5f,-0.5f);	// Top Right Of The Quad (Right)
    glVertex3f( 0.5f, 0.5f, 0.5f);	// Top Left Of The Quad (Right)
    glVertex3f( 0.5f,-0.5f, 0.5f);	// Bottom Left Of The Quad (Right)
    glVertex3f( 0.5f,-0.5f,-0.5f);	// Bottom Right Of The Quad (Right)
  glEnd();			// End Drawing The Cube
}

void drawBox(GLfloat size, GLenum type)
{
	static GLfloat n[6][3] = {
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] = {
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2.f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2.f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2.f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2.f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2.f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2.f;

	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glVertex3fv(&v[faces[i][0]][0]);
		glVertex3fv(&v[faces[i][1]][0]);
		glVertex3fv(&v[faces[i][2]][0]);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}

void glutWireCube(GLdouble size)
{
	drawBox(size, GL_LINE_LOOP);
}

void glutSolidCube(GLdouble size)
{
	drawBox(size, GL_QUADS);
}
