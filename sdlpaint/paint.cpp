/**
 * Copyright (C) 2007 Christian B. Ries
 * License: MIT
 * Website: https://github.com/cbries/utilities
 */
 
/**
 * System Header
 */
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

/**
 * eigene Headers
 */
#include "config.h"
#include "error.h"

// Widget Framework
#include "robject.h"
#include "rframe.h"
#include "rbutton.h"
#include "rcolor.h"
#include "mousecoord.h"
#include "rpreview_line.h"

using namespace std;

Uint32 VIDEOFLAGS = SDL_HWSURFACE |
					SDL_HWPALETTE |
                    SDL_DOUBLEBUF |
                    SDL_RESIZABLE;

MouseCoord MC;

RFrame  * frameBtn;
RButton * button1;
RButton * button2;
RButton * button3;
RButton * button4;
RFrame  * frameColor;
RColor  * color[20];
RColor  * colorChoice;
RFrame  * frameDrawing;
RFrame  * frameAttributes;
RButton * attr_line1;
RButton * attr_line2;
RButton * attr_line3;
RPreviewLine * pline;

enum CALLBACKTYPE { TYPE, ATTR };
enum DRAWINGTYPE { PENCIL, LINE, ELLIPSE, RECTANGLE };
enum DRAWINGATTR { THICK_SMALL, THICK_MIDDLE, THICK_BIG };

DRAWINGTYPE drawing_type = PENCIL;
DRAWINGATTR drawing_attr = THICK_SMALL;

void * handle_mouse_callback( int type, int type_value )
{
	if( (CALLBACKTYPE)type == TYPE ) drawing_type = (DRAWINGTYPE)type_value;
	if( (CALLBACKTYPE)type == ATTR ) drawing_attr = (DRAWINGATTR)type_value;
}

void handle_keyboard_input( SDLKey sym )
{
	switch(sym) {
		case 'q':
		case 'Q':
			cout << "Beenden" << endl;
			exit(0);
		break;

		case 'h':
		case 'H':
			cout << "Copyright (C) 2007 Christian B. Ries" << endl;
			cout << "Contact: Christian_Benjamin.Ries@fh-bielefeld.de" << endl;
			cout << endl;
			cout << "Dieses Programm stellt eine Demonstration fuer die" << endl;
			cout << "GD Vorlesung von Prof. Dr. Bunse dar." << endl;
			cout << endl;
			cout << "SDL Version: " << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << endl;
		break;

		case 'm':	
		case 'M':
			SDL_Rect ** modes;
			int i;

			/* Get available fullscreen/hardware modes */
			modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

			/* Check is there are any modes available */
			if(modes == (SDL_Rect **)0){
			  printf("No modes available!\n");
			  exit(-1);
			}

			/* Check if or resolution is restricted */
			if(modes == (SDL_Rect **)-1){
			  printf("All resolutions available.\n");
			}
			else{
			  /* Print valid modes */
			  printf("Available Modes\n");
			  for(i=0;modes[i];++i)
			    printf("  %d x %d\n", modes[i]->w, modes[i]->h);
			}
		break;
	}
}

const SDL_Surface * create_formular( const SDL_Surface * surface, int w, int h )
{
	surface = SDL_SetVideoMode( w, h, 32, VIDEOFLAGS );

	boxRGBA( (SDL_Surface*)surface, 0, 0, w, h, 255, 255, 255, 255 );

	MC.clear();

	SDL_Rect coord;

	/***********************************************************************
	 * Aktionsschalter
	 */

	frameBtn = new RFrame( surface, 
			5, 	// x start
			5, 	// y start
			62,		// width
			200		// height
		  );
	frameBtn->draw();

	button1 = new RButton( surface, 
			8, 8, 26, 26 );
	button1->setIcon("icons/paint.xpm");
	button1->setName("pencil");
	button1->setCallbackType( TYPE, PENCIL );
	button1->setCallbackFunction( handle_mouse_callback );
	button1->draw();
	coord.x = 8;
	coord.y = 8;
	coord.w = 26;
	coord.h = 26;
	MC.addArea( coord, (RObject*)button1 );

	button2 = new RButton( surface,
			36, 8, 26, 26 );
	button2->setIcon("icons/line.xpm");
	button2->setName("line");
	button2->setCallbackType( TYPE, LINE );
	button2->setCallbackFunction( handle_mouse_callback );
	button2->draw();
	coord.x = 36;
	coord.y = 8;
	coord.w = 26;
	coord.h = 26;
	MC.addArea( coord, (RObject*)button2 );

	button3 = new RButton( surface,
			8, 36, 26, 26 );
	button3->setIcon("icons/ellipse.xpm");
	button3->setName("ellipse");
	button3->setCallbackType( TYPE, ELLIPSE );
	button3->setCallbackFunction( handle_mouse_callback );
	button3->draw();
	coord.x = 8;
	coord.y = 36;
	coord.w = 26;
	coord.h = 26;
	MC.addArea( coord, (RObject*)button3 );

	button4 = new RButton( surface,
			36, 36, 26, 26 );
	button4->setIcon("icons/rectangle.xpm");
	button4->setName("rectangle");
	button4->setCallbackType( TYPE, RECTANGLE );
	button4->setCallbackFunction( handle_mouse_callback );
	button4->draw();
	coord.x = 36;
	coord.y = 26;
	coord.w = 26;
	coord.h = 26;
	MC.addArea( coord, (RObject*)button4 );

	/************************************************************************
	 * Farbauswahl 
	 */

	int color_table[20][4] = {
			{ 0, 0, 0, 255 },
			{ 255, 0, 0, 255 },
			{ 0, 255, 0, 255 },
			{ 0, 0, 255, 255 },
			{ 255, 255, 0, 255 },
			{ 0, 255, 255, 255 },
			{ 255, 255, 255, 255 },
			{ 255, 0, 255, 255 },
			{ 125, 125, 125, 255 },
			{ 55, 55, 55, 255 },
	
			{ 33, 33, 33, 255 },
			{ 204, 204, 204, 255 },
			{ 88, 88, 88, 255 },
			{ 34, 0, 234, 255 },
			{ 99, 99, 99, 255 },
			{ 155, 155, 155, 255 },
			{ 0, 32, 133, 255 },
			{ 12, 24, 48, 255 },
			{ 90, 90, 255, 255 },
			{ 211, 211, 211, 255 }
		};

	frameColor = new RFrame( surface, 
			5, h - 67,
			286, 62 );
	frameColor->draw();

	for( int i=0; i < 10; i++ ) {
		color[i] = new RColor( surface, 8 + (i*28), h - 64, 26, 26 );
		color[i]->setRGBA( color_table[i][0],
						   color_table[i][1],
						   color_table[i][2],
						   color_table[i][3] );
		color[i]->draw();
		coord.x = 8 + (i*28);
		coord.y = h - 64;
		coord.w = 26;
		coord.h = 26;
		MC.addArea( coord, color[i] );
	}

	for( int i=10; i < 20; i++ ) {
		color[i] = new RColor( surface, 8 + ((i-10)*28), h - 36, 26, 26 );
		color[i]->setRGBA( color_table[i][0],
						   color_table[i][1],
						   color_table[i][2],
						   color_table[i][3] );
		color[i]->draw();
		coord.x = 8 + ((i-10)*28);
        coord.y = h - 36;
        coord.w = 26;
        coord.h = 26;
        MC.addArea( coord, color[i] );
	}

	colorChoice = new RColor( surface, 300, h - 67, 62, 62 );
	colorChoice->draw();

	/******************************************************************
 	 * Malflaeche
 	 */

	frameDrawing = new RFrame( surface,
							80, 5, w - 5 - 80, h - 75 - 5 );
	frameDrawing->setRGBA( 255, 255, 255, 255 );
	frameDrawing->setName("frameDrawing");
	frameDrawing->setType(PAINTING);
	frameDrawing->draw();
	coord.x = 80;
	coord.y = 5;
	coord.w = w - 5 - 80;
	coord.h = h - 75 - 5;
	MC.addArea( coord, (RObject*)frameDrawing );

	/******************************************************************
 	 * Attribute
 	 */
	frameAttributes = new RFrame( surface,
							15, 210, 42, 110 );
	frameAttributes->draw();

	attr_line1 = new RButton( surface, 22, 218, 28, 26 );
	attr_line1->setIcon("icons/linedicke1.xpm");
	attr_line1->setName("attr_line");
	attr_line1->setCallbackType( ATTR, THICK_BIG );
	attr_line1->setCallbackFunction( handle_mouse_callback );
	attr_line1->draw();
	coord.x = 22;
	coord.y = 218;
	coord.w = 28;
	coord.h = 26;
	MC.addArea( coord, (RObject*)attr_line1 );

	attr_line2 = new RButton( surface, 22, 250, 28, 26 );
	attr_line2->setIcon("icons/linedicke2.xpm");
	attr_line2->setName("attr_line");
	attr_line2->setCallbackType( ATTR, THICK_MIDDLE );
	attr_line2->setCallbackFunction( handle_mouse_callback );
	attr_line2->draw();
	coord.x = 22;
	coord.y = 250;
	coord.w = 28;
	coord.h = 26;
	MC.addArea( coord, (RObject*)attr_line2 );

	attr_line3 = new RButton( surface, 22, 282, 28, 26  );
	attr_line3->setIcon("icons/linedicke3.xpm");
	attr_line3->setName("attr_line");
	attr_line3->setCallbackType( ATTR, THICK_SMALL );
	attr_line3->setCallbackFunction( handle_mouse_callback );
	attr_line3->draw();
	coord.x = 22;
	coord.y = 282;
	coord.w = 28;
	coord.h = 26;
	MC.addArea( coord, (RObject*)attr_line3 );

	/******************************************************************
	 * Ende 
	 */

	SDL_UpdateRect( (SDL_Surface*)surface, 0, 0, w, h );

	return surface;
}

void draw_point_area( const SDL_Surface * surface, int x, int y )
{
	int r, g, b, a;
	colorChoice->RGBA( &r, &g, &b, &a );

	boxRGBA( (SDL_Surface*) surface, x - 1, y - 1,
        x + 1, y + 1,
		r, g, b, a );
    SDL_UpdateRect((SDL_Surface*)surface, x - 1, y - 1, 3, 3 );
}

int main( int argc, char **argv )
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
		sdlFatalError("Konnte SDL nicht initialisieren");
	}
	atexit(SDL_Quit);

	string wm_title = PROGRAMMNAME;
	wm_title += " v";
	wm_title += VERSION;
	wm_title += " - Copyright (C) 2007 Christian B. Ries | FH-Bielefeld";

	SDL_WM_SetCaption( wm_title.c_str(), NULL );

	const SDL_Surface * surface = SDL_SetVideoMode( WIN_WIDTH, WIN_HEIGHT, 32, VIDEOFLAGS );
	if( surface == NULL ) {
		sdlFatalError("Konnte die Zeichenflaeche nicht initialisieren");
	}

	surface = create_formular(surface, WIN_WIDTH, WIN_HEIGHT);

	/**
 	 * aktuelle Status fuer die Zeichenaktionen.
 	 */
	bool state_drawing   = false;		// true, wenn Pencil ausgewaehlt wurde.
	bool state_mousedown = false;		// true, wenn die Maustaste gedrueckt gehalten wird.
	bool state_point1    = false;		// true, wenn schon ein Punkt angeklickt wurde.
										// Wird zu false zurueck gesetzt wenn zweiter
										// Klick gemacht wurde.

	SDL_Event event;
	while(1)
	{

	// SDL_WaitEvent(&event), System blockiert
	// SDL_PollEvent(&event), stetiges Abfragen
	int poll_ret = 0;
	while( (poll_ret=SDL_PollEvent(&event))) {

		#define KEYEvent event.key
		#define MOTIONEvent event.motion
		#define MOUSEEvent event.button
		#define RESIZEEvent event.resize

		if( poll_ret == 0 )
			continue;

		SDL_Rect clip;
		clip.x = 0;
		clip.y = 0;
		clip.w = WIN_WIDTH;
		clip.h = WIN_HEIGHT;
		SDL_SetClipRect( (SDL_Surface*)surface, &clip );

		switch(event.type) {
			// Tastatur
			case SDL_KEYDOWN: 
			break;
			case SDL_KEYUP: 
				if( KEYEvent.state == SDL_RELEASED ) {
					handle_keyboard_input(KEYEvent.keysym.sym);
				}
			break;
			
			// Maus Bewegung
			case SDL_MOUSEMOTION: { 
				RObject * objValid = MC.isValid( MOUSEEvent.x, MOUSEEvent.y );
				if( objValid != NULL ) {
					if( objValid->type() == PAINTING ) {
						if( state_drawing )
						{
							draw_point_area( surface, MOUSEEvent.x, MOUSEEvent.y );
						}
			
						if( state_mousedown )
						{
							pline->newEnd( MOUSEEvent.x, MOUSEEvent.y );
						}
					}
				}
			} break;

			// Maus Aktivitaet
			case SDL_MOUSEBUTTONDOWN: {
				RObject * objValid = MC.isValid( MOUSEEvent.x, MOUSEEvent.y );
                if( objValid != NULL ) { 
               		switch(objValid->type())
      				{
                		case BUTTON: {
                    	        RButton * btn = (RButton*) objValid;
                    	        btn->toggle(true);
                    	        btn->draw();
                    	} break;
                    	case PAINTING: {
							switch(drawing_type)
							{
							  case PENCIL: {
								state_drawing = true;
								draw_point_area( surface, MOUSEEvent.x, MOUSEEvent.y );
                    		  } break;
							  case LINE: {
								state_mousedown = true;
								pline = new RPreviewLine( (SDL_Surface*)surface, MOUSEEvent.x, MOUSEEvent.y );
							  } break;
							  case ELLIPSE: {
								state_mousedown = true;
							    pline = new RPreviewLine( (SDL_Surface*)surface, MOUSEEvent.x, MOUSEEvent.y );
							  } break;
							  case RECTANGLE: {
								state_mousedown = true;
								pline = new RPreviewLine( (SDL_Surface*)surface, MOUSEEvent.x, MOUSEEvent.y );
							  } break;
							}
						} break;
                	}
				}
			} break;
			case SDL_MOUSEBUTTONUP: {

				state_drawing = false;
				state_mousedown = false;

				RObject * objValid = MC.isValid( MOUSEEvent.x, MOUSEEvent.y );
                if( objValid != NULL ) { 
					switch(objValid->type())
					{
						case BUTTON: {
							RButton * btn = (RButton*) objValid;

							if( btn->name() == "attr_line" )
							{
								attr_line1->toggle(false);
                            	attr_line1->draw();
                            	attr_line2->toggle(false);
                            	attr_line2->draw();
                            	attr_line3->toggle(false);
                            	attr_line3->draw();

								btn->toggle(true);
								btn->Callback();
								btn->draw();
							} else {
								btn->toggle(false);
								btn->Callback();
                            	btn->draw();
							}
						} break;
						case COLOR: {
							RColor * color = (RColor*) objValid;
							int r, g, b, a;
							color->RGBA( &r, &g, &b, &a );
							colorChoice->setRGBA( r, g, b, a );
							colorChoice->draw();
						} break;
						case PAINTING: {
							SDL_Rect rect;

							if ( pline != NULL )
							{
								pline->stop();
								rect = pline->rect();

								delete pline;
								pline = NULL;
							}

								int r, g, b, a;
                            	colorChoice->RGBA( &r, &g, &b, &a );

							switch(drawing_type) {
								case LINE: {
									int width;
		
									switch(drawing_attr) {
										case THICK_SMALL: width = 7; break;
										case THICK_MIDDLE: width = 15; break;
										case THICK_BIG: width = 21; break;
										default: width: 3;
									}

									for( int i = 0; i<width/2; i++ )
										aalineRGBA( (SDL_Surface*)surface, rect.x - i, rect.y - i,
                                                         	rect.x + rect.w - i, rect.y + rect.h - i,
                                                        	r, g, b, a );	
									aalineRGBA( (SDL_Surface*)surface, rect.x, rect.y, 
														 rect.x + rect.w, rect.y + rect.h,
														r, g, b, a );
									for( int i = 0; i<width/2; i++ )
										aalineRGBA( (SDL_Surface*)surface, rect.x + i, rect.y + i,
                                                         	rect.x + rect.w + i, rect.y + rect.h + i,
                                                        	r, g, b, a );
								} break;
							    case ELLIPSE: {
									filledEllipseRGBA( (SDL_Surface*)surface, rect.x + (rect.w/2), rect.y + (rect.h/2),
															rect.w / 2, rect.h / 2,
															r, g, b, a );
								} break;
								case RECTANGLE: {
									boxRGBA( (SDL_Surface*)surface, rect.x, rect.y,
																	rect.x + rect.w, rect.y + rect.h,
																	r, g, b, a );
								} break;	
							}

							SDL_UpdateRect( (SDL_Surface*)surface, rect.x, rect.y, rect.w + 1, rect.h + 1 );
						} break;
					}
				}
			} break;
		
			// Resize
			case SDL_VIDEORESIZE: 
				//cout << "VIDEORESIZE" << endl;
				cout << "x: " << event.resize.w << ", y: " << event.resize.h << endl;
				//surface = create_formular(surface, event.resize.w, event.resize.h);
			break;
		}
	}

	} // while(1)

	SDL_Quit();
}

