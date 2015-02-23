/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __RPREVIEW_LINE_H__
#define __RPREVIEW_LINE_H__

#pragma once

// System
#include <SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

// eigene
#include "robject.h"

class RPreviewLine
{
public:
	RPreviewLine() { _counter = 0; }
	RPreviewLine( SDL_Surface * src, int x, int y ) {
		_counter = 0;
		_src = src;
		_start.x = x;
		_start.y = y;

		_face = SDL_CreateRGBSurface( SDL_HWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA, 
				640, 480, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );

		_r_old.x = 0;
		_r_old.y = 0;
		_r_old.w = 640;
		_r_old.h = 480;

		if( SDL_BlitSurface( _src, NULL, _face, NULL ) != 0 )
			cout << "Blit 1 failed: " << SDL_GetError() << endl;
	}

	// TODO: Performance erhoehen!
	void newEnd( int x, int y ) {

		if( _counter != 5 ) {
			_counter++;
			return;
		}
		
		_counter = 0;	

		//SDL_BlitSurface( _face, &_r_old, _src, &_r_old );
		SDL_BlitSurface( _face, NULL, _src, NULL );
	
		SDL_Rect r;
	
		if( x >= _start.x ) { r.x = _start.x;  r.w = x - _start.x; }
		if( x <  _start.x ) { r.x = x; r.w = _start.x - x; }
		
		if( y >= _start.y ) { r.y = _start.y;  r.h = y - _start.y; }
		if( y <  _start.y ) { r.y = y; r.h = _start.y - y; }

		_r_old = r;

		rectangleRGBA( _src, r.x, r.y, r.x + r.w, r.y + r.h, 0, 0, 0, 255 );

		SDL_UpdateRect( _src, 0, 0, 640, 480 );		
	}

	void stop() {
		SDL_BlitSurface( _face, NULL, _src, NULL );
		SDL_UpdateRect( _src, 0, 0, 640, 480 );	
		//SDL_UpdateRect( _src, _r_old.x, _r_old.y, _r_old.w+3, _r_old.h+3 );
	}

	SDL_Rect rect() {
		return _r_old;
	}
	
private:
	point _start;
	SDL_Rect _r_old;
	SDL_Surface * _src;
	SDL_Surface * _face;
	int _counter;
};

#endif

