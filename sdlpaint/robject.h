/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __ROBJECT_H__
#define __ROBJECT_H__

#include <unistd.h>
#include <iostream>
#include <string>
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;

enum TYPE { BUTTON, FRAME, COLOR, PAINTING, NOTHING };

struct point {
	uint x;
	uint y;
};

class RObject
{
public:
	RObject();
	RObject( const SDL_Surface *, int, int, int, int );
	~RObject();

	void position( int *, int * );
	point position();

	int x();
	int y();
	int width();
	int height();

	int x_end();
	int y_end();

	void setName( const string & );
	void setName( const char * );
	string name();

	void setDescription( const string & );
	void setDescription( const char * );
	string description();

	void setRect( SDL_Rect );
	SDL_Rect rect();

	void setSurface( SDL_Surface * s );
	const SDL_Surface * surface();

	void setType( TYPE t );
	TYPE type();

	void setCallbackType( int type, int value ) { 
		_callback_type = type; 
		_callback_value = value;
	}

	void setCallbackFunction( void *(*ptr_func)(int,int) ) {
		_ptr_func = ptr_func;
	}

	void Callback() {
		(*_ptr_func)(_callback_type, _callback_value);
	}
	
	virtual void draw() = 0;

private:
	TYPE _type;
	SDL_Rect _rect;
	const SDL_Surface * _surface;
	string _name;
	string _description;	

	int _callback_type;
	int _callback_value;

	void *(*_ptr_func)(int,int);
}; 

#endif
