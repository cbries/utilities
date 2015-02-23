/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#include "rcolor.h"

RColor::RColor()
{
	setType( COLOR );
}

RColor::RColor( const SDL_Surface * surface,
				int xpos, int ypos,
				int width, int height )
	: RButton( surface, xpos, ypos, width, height )
{
	setType( COLOR );
}

RColor::~RColor()
{
}

void RColor::draw()
{
	SDL_Rect clip;
	clip.x = x();
	clip.y = y();
	clip.w = width();
	clip.h = height();
	SDL_SetClipRect((SDL_Surface*)surface(), &clip);

	if( _state == NOTACTIVE )
	{
		boxRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, _r, _g, _b, 255 );

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
		boxRGBA( (SDL_Surface*)surface(), x() + 2, y() + 2, x_end() - 2, y_end() - 2, _r, _g, _b, 255 );
	
		rectangleRGBA( (SDL_Surface*)surface(), x() + 2, y() + 2, x_end() - 2, y_end() - 2, 0, 0, 0, 255 );
	
		aalineRGBA( (SDL_Surface*)surface(), x(), y() + 0, x_end() - 3, y() + 0, 0, 0, 0, 255 );
        aalineRGBA( (SDL_Surface*)surface(), x(), y() + 1, x_end() - 3, y() + 1, 0, 0, 0, 255 );
        aalineRGBA( (SDL_Surface*)surface(), x(), y() + 2, x_end() - 3, y() + 2, 0, 0, 0, 255 );

        aalineRGBA( (SDL_Surface*)surface(), x() + 0, y(), x() + 0, y_end() - 3, 0, 0, 0, 255 );
        aalineRGBA( (SDL_Surface*)surface(), x() + 1, y(), x() + 1, y_end() - 3, 0, 0, 0, 255 );
        aalineRGBA( (SDL_Surface*)surface(), x() + 2, y(), x() + 2, y_end() - 3, 0, 0, 0, 255 );
	}

	SDL_UpdateRect((SDL_Surface*)surface(), x(), y(), width(), height() );
}
