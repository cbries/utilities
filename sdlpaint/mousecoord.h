/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __MOUSECOORD_H__
#define __MOUSECOORD_H__

#include <SDL.h>
#include <vector>
#include <iostream>
#include "robject.h"

using namespace std;

struct MouseObject
{
	SDL_Rect area;
	RObject * eventObject;
};

class MouseCoord
{
public:
	MouseCoord();
	void addArea( SDL_Rect, RObject * );
	RObject * isValid( int, int );
	void clear();
private:
	vector<MouseObject> objectList;
};

#endif
