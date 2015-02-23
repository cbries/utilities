/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
#include "mousecoord.h"

MouseCoord::MouseCoord()
{	
}

void MouseCoord::addArea( SDL_Rect area, RObject * object )
{
	MouseObject mouse;
	mouse.area = area;
	mouse.eventObject = object;
	objectList.push_back(mouse);
}

RObject * MouseCoord::isValid( int x, int y )
{
	if( objectList.size() <= 0 )
		return NULL;

	bool ok1 = false, ok2 = false;

	vector<MouseObject>::iterator it;
	for( it=objectList.begin(); it!=objectList.end(); it++ )
	{
		ok1 = ok2 = false;

		if( x >= it->area.x 		&&		x <= (it->area.x + it->area.w) )
			ok1 = true;
		else
			ok1 = false;

		if( y >= it->area.y			&&		y <= (it->area.y + it->area.h) )
			ok2 = true;
		else
			ok2 = false;

		if( ok1 && ok2 )
			return (it->eventObject);
	}
	return NULL;
}

void MouseCoord::clear()
{
	objectList.clear();
}
