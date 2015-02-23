/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#include "rframe.h"

RFrame::RFrame()
{
	_r = _g = _b = 154;
	_a = 255;
}

RFrame::RFrame( const SDL_Surface * surface,
				int xpos, int ypos,
				int width, int height )
	: RObject( surface, xpos, ypos, width, height )
{
	_r = _g = _b = 154;
	_a = 255;
}

void RFrame::setRGBA( int r, int g, int b, int a )
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void RFrame::addButton( RButton * btn )
{
	buttonList.push_back(btn);
}

void RFrame::rstButton()
{
	vector<RButton*>::iterator it;
	for( it=buttonList.begin(); it!=buttonList.end(); it++ )
	{
		RButton * btn;
		btn = *it;
		btn->toggle(false);
		btn->draw();
	}
}

void RFrame::draw()
{
	SDL_Rect clip;
	clip.x = x();
	clip.y = y();
	clip.w = width();
	clip.h = height();
	SDL_SetClipRect((SDL_Surface*)surface(), &clip);

	boxRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, _r, _g, _b, _a );

	rectangleRGBA( (SDL_Surface*)surface(), x(), y(), x_end()-2, y_end()-2, 0, 0, 0, 255 );

	aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 2, x_end(), y_end() - 2, 0, 0, 0, 255 );
	aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 1, x_end(), y_end() - 1, 0, 0, 0, 255 );
	aalineRGBA( (SDL_Surface*)surface(), x() + 2, y_end() - 0, x_end(), y_end() - 0, 0, 0, 0, 255 );

	aalineRGBA( (SDL_Surface*)surface(), x_end() - 2, y() + 2, x_end() - 2, y_end() - 2, 0, 0, 0, 255 );
	aalineRGBA( (SDL_Surface*)surface(), x_end() - 1, y() + 2, x_end() - 1, y_end() - 2, 0, 0, 0, 255 );
	aalineRGBA( (SDL_Surface*)surface(), x_end() - 0, y() + 2, x_end() - 0, y_end() - 2, 0, 0, 0, 255 );

	SDL_UpdateRect((SDL_Surface*)surface(), x(), y(), width(), height() );
}
