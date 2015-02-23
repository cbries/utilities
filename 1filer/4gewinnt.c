/*
 * Copyright (C) 2002, Christian Benjamin Ries
 * License: MIT
 * Date: 25. May 2002
 */

//
// Build: $ g++ -O3 -o 4gewinnt 4gewinnt.c
//
 
#include <stdio.h>
#include <stdlib.h>
#define CLS (printf("\033[2J"))
#define LOCATE(z,s)	(printf("\033[%i;%iH", z, s))
#define COLOR (v,h)	(printf("\033[1;3 %i ;4 %i m", v, h))
#define NORMAL (printf("\033[0m"))

/* Farben */
#define BLACK	0
#define RED	1
#define GREEN	2
#define YELLOW	3
#define BLUE	4
#define MAGENTA	5
#define CYAN	6
#define WHITE	7

/* User Macros */
#define NOBODY  -1
#define USER1	1
#define USER2	2

/* Funktions Deklarationen */
void welcome();
void spielen();
void copy();

int XLINE	= 10;
int YLINE	= 10;

int main( int argc, char *argv[])
{
	int zeichen;

	CLS; LOCATE(0,0);
	printf("\033[1;3%i;4%im", RED, BLACK);
	welcome();
	zeichen = fgetc(stdin);
	if(zeichen == 'q')
	{
		exit(0);
		NORMAL;
	}
	else
		printf("Spielen\n");	
	
	spielen(); 
	
	NORMAL;
	return 0;
}

void welcome()
{
	printf("****************************************\n");
	printf("*        4Gewinnt text based           *\n");
	printf("*                                      *\n");
	printf("* Press any key ... ([q] Quit)         *\n");
	printf("****************************************\n");
}

void copy()
{
	printf("*** :: 4Gewinnt :: ***\n\n");
}

void spielen()
{
	int i, j;
	int gewinn 	= 1;
	int zug 	= -1;
	int Gewinner 	= NOBODY;
	int Player	= USER1;
	int counter	= 0;

	CLS; LOCATE(0,0);
	int feld[YLINE][10];
	for(i=0; i<YLINE; i++)
		for(j=0; j<XLINE; j++)
			feld[i][j] = 0;
	
   while(gewinn < YLINE && gewinn > -1)
   {   
   	if(Player == USER1)
		  Player = USER2;
	  else if(Player == USER2)
		  Player = USER1;
    
   	if( zug > -1 )
   	{
   		for(i=YLINE ; i>-1; i--)
   		{
   			printf("%i : Here2 !\n", i);
   			if( feld[i][zug] == 0 )
   			{
			       	if(Player == USER1)
   				{	feld[i][zug] = 1;
   					break;	}
   				else if(Player == USER2)
   				{	feld[i][zug] = 2;
   					break;	} 
   			}	
   		}
   	}	
   		   	
   	CLS; LOCATE(0,0);
   	printf("\033[1;3%i;4%im", BLUE, BLACK);
   		copy();
   	printf("\033[1;3%i;4%im", RED, BLACK);
	for(i=0; i<YLINE; i++)
	{
		for(j=0; j<XLINE; j++)
		{
			if(feld[i][j] == 1)
				printf("\033[1;3%i;4%im", GREEN, BLACK);
			else if(feld[i][j] == 2)
				printf("\033[1;3%i;4%im", YELLOW, BLACK);	
			printf("%i ", feld[i][j]);		
				printf("\033[1;3%i;4%im", RED, BLACK);
		}
		printf(" |\n");
	}
	for(i=0; i<XLINE; i++)
		printf("--");
	printf("--\n");
	for(i=0; i<XLINE; i++)
		printf("%i ", i);	
   	if(Player == USER2)
   	{
   		printf("\033[1;3%i;4%im", GREEN, BLACK);
   		printf("\n\nUSER1 ist am Zug!");
   		printf("\033[1;3%i;4%im", RED, BLACK);
   	}
   	else if(Player == USER1)
   	{
   		printf("\033[1;3%i;4%im", YELLOW, BLACK);
   	 	printf("\n\nUSER2 ist am Zug!");	
   		printf("\033[1;3%i;4%im", RED, BLACK);
   	}
   	else
   		printf("Fehler!\n");

	/* Prüfen ob einer gewonnen hat _START_ */
	/****************************************/
	for( i=0; i<YLINE; i++)
		for( j=0; j<XLINE; j++)
		{
			/* Kontrolle ob einer Senkrecht 4 nebeneinander hat. */    
			if( 	feld[i][j] == 1 &&
				feld[i+1][j] == 1 &&
				feld[i+2][j] == 1 &&
				feld[i+3][j] == 1 )
			    Gewinner = USER1;
			if( 	feld[i][j] == 2 &&
				feld[i+1][j] == 2 &&
				feld[i+2][j] == 2 &&
				feld[i+3][j] == 2 )
			    Gewinner = USER2;
			    
			/* Kontrolle ob einer Waagerecht 4 nebeneinander hat. */    
			if( 	feld[i][j] == 1 &&
				feld[i][j+1] == 1 &&
				feld[i][j+2] == 1 &&
				feld[i][j+3] == 1 )
			    Gewinner = USER1;
			if( 	feld[i][j] == 2 &&
				feld[i][j+1] == 2 &&
				feld[i][j+2] == 2 &&
				feld[i][j+3] == 2 )
			    Gewinner = USER2;
			
		    if( i < 7 && j < 7 )
		    {			    
			/* Kontrolle ob einer Diagonal 4 nebeneinander hat. */    
			if( 	feld[i][j] == 1 &&
				feld[i+1][j+1] == 1 &&
				feld[i+2][j+2] == 1 &&
				feld[i+3][j+3] == 1 )
			    Gewinner = USER1;
			if( 	feld[i][j] == 2 &&
				feld[i+1][j+1] == 2 &&
				feld[i+2][j+2] == 2 &&
				feld[i+3][j+3] == 2 )
			    Gewinner = USER2;
			    
			if( 	feld[i][j] == 1 &&
				feld[i+1][j-1] == 1 &&
				feld[i+2][j-2] == 1 &&
				feld[i+3][j-3] == 1 )
			    Gewinner = USER1;
			if( 	feld[i][j] == 2 &&
				feld[i+1][j-1] == 2 &&
				feld[i+2][j-2] == 2 &&
				feld[i+3][j-3] == 2 )
			    Gewinner = USER2;	
		    }
		}
		
	/****************************************/
	/* Prüfung ob einer gewonnen hat _ENDE_ */

	if(Gewinner == NOBODY)
	{   	printf("\n\nEingabe: "); int n = scanf("%i", &zug); }
	else if(Gewinner == USER1)
	{
		printf("\n\nUSER1 hat gewonnen !\n");
		break;
	}
	else if(Gewinner == USER2)
	{
		printf("\n\nUSER2 hat gewonnen !\n");
		break;
	}
   	gewinn = zug;
   }
   if(Gewinner == USER1)
   	printf("Herzlichen Glückwunsch User1\n");
   else if(Gewinner == USER2)
   	printf("Herzlichen Glückwunsch User2\n");
   else
   	printf("Keiner hat gewonnen!\n");
}

