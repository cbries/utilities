/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#include "rbutton.h"

RButton::RButton()
{
	setType( BUTTON );
    _state = NOTACTIVE;
    _icon = NULL;
    _r = 204;
    _g = 204;
    _b = 204;
    _a = 255;
}

RButton::RButton( const SDL_Surface * surface,
				int xpos, int ypos,
				int width, int height )
	: RObject( surface, xpos, ypos, width, height )
{
	setType( BUTTON );
	_state = NOTACTIVE;
	_icon = NULL;
	_r = 204;
	_g = 204;
	_b = 204;
	_a = 255;
}

RButton::~RButton()
{
	if( _icon != NULL )
		SDL_FreeSurface(_icon);
}

void RButton::toggle( bool s )
{
	if(!s)	_state = NOTACTIVE;
	else	_state = ACTIVE;
}

void RButton::toggle()
{
	if(_state == ACTIVE )	_state = NOTACTIVE;
	else					_state = ACTIVE;
} 

void RButton::setIcon( const char * filename )
{
	if( filename == NULL ) return;

	string f(filename);
	_filename = f;

	if( _icon != NULL )
		SDL_FreeSurface(_icon);
	
	_icon = IMG_Load(filename);
	if( _icon == NULL ) {
		sdlError("Konnte das Bild nicht laden");
		return;
	}

	updateIcon();
}

void RButton::setRGBA( int r, int g, int b, int a )
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}	

void RButton::RGBA( int * r, int * g, int * b, int * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void RButton::updateIcon()
{
	SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = 22;
    src_rect.h = 22;

    SDL_Rect dst_rect;
    dst_rect.x = x() + 2;
    dst_rect.y = y() + 2;
    dst_rect.w = 22;
    dst_rect.h = 22;

	// TODO:
	// Skalierung des Bildes.
    SDL_BlitSurface( _icon, &src_rect,
                     (SDL_Surface*)surface(), &dst_rect );
}

void RButton::draw()
{
	SDL_Rect clip;
	clip.x = x();
	clip.y = y();
	clip.w = width();
	clip.h = height();
	SDL_SetClipRect((SDL_Surface*)surface(), &clip);

	if( _state == NOTACTIVE )
	{
		boxRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, _r, _g, _b, _a );

		rectangleRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, 0, 0, 0, 255 );

		aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 2, x_end(), y_end() - 2, 0, 0, 0, 255 );
		aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 1, x_end(), y_end() - 1, 0, 0, 0, 255 );
		aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 0, x_end(), y_end() - 0, 0, 0, 0, 255 );
	
		aalineRGBA( (SDL_Surface*)surface(), x_end() - 2, y() + 2, x_end() - 2, y_end() - 2, 0, 0, 0, 255 );
		aalineRGBA( (SDL_Surface*)surface(), x_end() - 1, y() + 2, x_end() - 1, y_end() - 2, 0, 0, 0, 255 );
		aalineRGBA( (SDL_Surface*)surface(), x_end() - 0, y() + 2, x_end() - 0, y_end() - 2, 0, 0, 0, 255 );
	}

	if( _state == ACTIVE )
	{
		boxRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, 0, 0, 0, _a );

		boxRGBA( (SDL_Surface*)surface(), x() + 1, y() + 1, x_end(), y_end(), _r, _g, _b, _a );
	
		rectangleRGBA( (SDL_Surface*)surface(), x() + 1, y() + 1, x_end()-1, y_end()-1, 0, 0, 0, 255 );
	}

	updateIcon();

	SDL_UpdateRect((SDL_Surface*)surface(), x(), y(), width(), height() );
}
