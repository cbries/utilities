/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __RBUTTON_H__
#define __RBUTTON_H__

#include "error.h"
#include "robject.h"

#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

enum STATE { NOTACTIVE, ACTIVE };

class RButton : public RObject
{
public:
	RButton();
	RButton( const SDL_Surface *, int, int, int, int );	
	~RButton();

	void toggle( bool s );
	void toggle();	

	void setIcon( const char * );

	void setRGBA( int, int, int, int );
	void RGBA( int *, int *, int *, int * );

	virtual void draw();
	
private:
	void updateIcon();
	
	SDL_Surface * _icon;
	string _filename;	// icon

protected:
	STATE _state;
	int _r, _g, _b, _a;
};

#endif
