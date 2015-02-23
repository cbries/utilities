/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __RCOLOR_H__
#define __RCOLOR_H__

#include "error.h"
#include "rbutton.h"

#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

class RColor : public RButton
{
public:
	RColor();
	RColor( const SDL_Surface *, int, int, int, int );	
	~RColor();

	void draw();
	
private:
};

#endif
