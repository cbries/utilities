/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#include "robject.h"

RObject::RObject()
{
	_type = NOTHING;
}

RObject::RObject( const SDL_Surface * surface,
					int xpos, int ypos,
					int width, int height )
{
	_surface = surface;
	_rect.x = xpos;
	_rect.y = ypos;
	_rect.w = width;
	_rect.h = height;
}

RObject::~RObject()
{
}

void RObject::position( int * x, int * y )
{
	*x = _rect.x;
	*y = _rect.y;
}

point RObject::position()
{
	point p;
	p.x = _rect.x;
	p.y = _rect.y;
	return p;
}

int RObject::x()		{ return _rect.x; }
int RObject::y() 		{ return _rect.y; }
int RObject::width() 	{ return _rect.w; }
int RObject::height()	{ return _rect.h; }

int RObject::x_end()	{ return (_rect.x + _rect.w); }
int RObject::y_end()	{ return (_rect.y + _rect.h); }

void RObject::setName( const string & n )
{
	_name = n;
}

void RObject::setName( const char * n )
{
	string _n(n);
	_name = _n;
}

string RObject::name()
{
	return _name;
}

void RObject::setDescription( const string & d )
{
	_description = d;
}

void RObject::setDescription( const char * d )
{
	string _d(d);
	_description = _d;
}

string RObject::description()
{
	return _description;
}

void RObject::setRect( SDL_Rect r )
{
	_rect = r;
}

SDL_Rect RObject::rect()
{
	return _rect;
}

void RObject::setSurface( SDL_Surface * s )
{
	_surface = s;
}

const 
SDL_Surface * RObject::surface()
{
	return _surface;
}

void RObject::setType( TYPE t )
{
	_type = t;
}

TYPE RObject::type()
{
	return _type;
}
