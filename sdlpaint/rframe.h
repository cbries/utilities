/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __RFRAME_H__
#define __RFRAME_H__

#include "robject.h"
#include "rbutton.h"

#include <vector>
#include <iostream>

using namespace std;

class RFrame : public RObject
{
public:
	RFrame();
	RFrame( const SDL_Surface *, int, int, int, int );	

	void setRGBA( int, int, int, int );

	void addButton( RButton * );
	void rstButton();

	void draw();
	
private:
	vector<RButton*> buttonList;

protected:
	int _r, _g, _b, _a;
};

#endif
