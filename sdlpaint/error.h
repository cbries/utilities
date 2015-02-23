/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#ifndef __ERROR_H__
#define __ERROR_H__

#include <SDL.h>
#include <iostream>

using namespace std;

inline
void sdlError( const char * errMessage )
{
    cerr << errMessage << ": " << SDL_GetError() << endl;
}

inline
void sdlFatalError( const char * errMessage )
{
    cerr << errMessage << ": " << SDL_GetError() << endl;
    exit(1);
}

#endif
