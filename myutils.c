#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
// #include "strfunz.c"

#ifndef strlower
#define strlower strlow
#endif

/* prototipi di funzione */
#include "myproto.h"

/* ------------------------------------------------------------------
 *           FUNZIONI DI UTILITA GENERALE
 * ------------------------------------------------------------------ */

int tira_numero_random( int finoa )
{
	int j;

	j=1+(int) ((float)finoa*rand()/(RAND_MAX+1.0));

	return( j );
}

/* Algoritmo di ordinamento crescente di un vettore "per inserimento" */
int ordina_vettore_intero(vettnum)
int vettnum[];
{
	int 		iIndiceA;
	int 		iIndiceB;
	int	numappo;

	for(iIndiceA = 1; iIndiceA < MAX_ELEM ; iIndiceA++)
	{
		numappo = vettnum[iIndiceA];

		for(iIndiceB = iIndiceA - 1
			; (iIndiceB >= 0) && (numappo < vettnum[iIndiceB])
			; iIndiceB--)
		{	
			vettnum[iIndiceB+1] = vettnum[iIndiceB];
		}	
		vettnum[iIndiceB + 1] = numappo;
	}
	
	/*
	** giro di controllo che determina quanti elementi esistono nel vettore
	** con valore > 0 
	iIndiceA = 0;
	while( vettnum[iIndiceA] > 0.0 ) iIndiceA++;
	return( iIndiceA );
	*/

	return( 0 );
}

int strpulall( char *str )
{
	strpul( str );
	pul( str );
	return strlen(str);
}

int pul( char *str )
{
	register int i;
	for(i=0;i<strlen(str);i++)
		if( str[i]=='\r' || str[i]=='\n' ) {
			str[i]=0;
			break;
		}

	return i;
}

int puldim( char *str, int size )
{
	register int i;
	for(i=0;i<size;i++)
	{
		if( str[i]==0 ) {
			str[i]=' ';
			continue;
		}
		if( str[i]=='\r' || str[i]=='\n' ) {
			str[i]=0;
			break;
		}
	}

	return i;
}

int _premi_un_tasto(char * messaggio) 
{
	char c;
	
	fprintf(stdout,(messaggio[0]==0) ? "Premere un tasto per Continuare.\n\r" : messaggio );
	fflush(stdout);
	c=getch();

	return c;
}

/*
 *     funzione msgalert
 */
void msgalert(char * msg)
{
		color_set( 15 ,NULL);
		mvprintw(22,0, msg );
		refresh();
		usleep(500000);
}

/*
 *     funzione msgerror
 */
void msgerror(char * msg)
{
		color_set( 1 ,NULL);
		mvprintw(22,0, msg );
		refresh();
		usleep(500000);
}

/*
 *     funzione disegna_box()
 */
int disegna_box( int starty, int startx, int altezza, int lunghezza, int colore_bordo, bool riempi, int colore_dentro , const char * titolo )
{
	int endy=0, endx=0;
	register int linea;

	endx=startx+lunghezza-1; 
	if( endx<=startx ) endx=startx+1;
	endy=starty+altezza-1; 
	if( endy<=starty ) endy=starty+1;

	color_set( colore_bordo ,NULL);
	mvaddch(starty,startx,ACS_ULCORNER);
	mvaddch(starty,endx,ACS_URCORNER);
	mvaddch(endy,startx,ACS_LLCORNER);
	mvaddch(endy,endx,ACS_LRCORNER);

	if( (endx - startx) > 1 ) {
		mvhline(starty, startx+1, ACS_HLINE, endx-startx-1 );
		mvhline(endy,   startx+1, ACS_HLINE, endx-startx-1 );
	}

	if( (endy - starty) > 1 ) {
		mvvline(starty+1, startx, ACS_VLINE, endy-starty-1 );
		mvvline(starty+1, endx,   ACS_VLINE, endy-starty-1 );
	}
	
	if( riempi && (endx - startx) > 1 && (endy - starty) > 1 ) {
		color_set( colore_dentro ,NULL);
		for( linea=starty+1;linea<endy;linea++)
			mvhline(linea,startx+1, ' ', endx-startx-1 );
	}

	if( !strvuota(titolo) ) {
		int ltit;
		char *buffer;

		if( (buffer=malloc(sizeof(titolo)+3))!=NULL ) {
			color_set( colore_bordo ,NULL);
			ltit=strlen(titolo);
			if((ltit+4)<lunghezza) {
				sprintf(buffer,"[%s]",titolo);
			} else {
				strcpy(buffer,titolo);
			}
			mvprintw(starty,startx+(lunghezza-strlen(buffer))/2,buffer);
		}
		free(buffer);
	}
	
	return 0;
}

/*
 *     funzione inputcontr( )
 */
int inputcontr( int coordy,
								int coordx, 
								char *out, 
								int maxlength, 
								const char * allowed_set, 
								const unsigned char mode,
								const char * z_predefinito,
								const char c_cursor )
{
	int c;
	int len=0;
	u8 modo;
	u8 cursore;

	curs_set(1);
	cursore = (c_cursor) ? c_cursor : STD_INPUT_CURSOR ;

	

	modo=toupper(mode);
	move(coordy,coordx);
	strncpy0(out,z_predefinito, MIN(maxlength,strlen(z_predefinito)));
	len=strlen(out);
	if(len) {
		if(mode=='L') strlow(out);
		if(mode=='U') strupper(out);
		printw("%s",out);
		coordx+=len;
	}

	do {
		move(coordy,coordx);
		if(len<maxlength)
			printw("%c",cursore);
		move(coordy,coordx);

		c=getch();
		switch(c) {
			/*
			 * Batto invio: salvo la stringa e la "taglio" alla sua lunghezza
			 */
			case 10: 
			case 13:
				if(len<maxlength) {
					printw(" ");
				}
				out[len]=0;
				break;
				
			/*
			 * Batto Backspace: devo tornare indietro e correggere la lunghezza
			 */
			case KEY_BACKSPACE:
			// case 127: /* FIXME: occorre leggere il tasto ritornato a sec del terminale */
				if(len) {
					printw(" ");
					coordx--;
					len--;
				}
				break;

			/*
			 * Qualsiasi altro tasto: Controllo che il carattere sia nel set permesso,
			 * se si, avanzo con la stringa d'uscita
			 */
			default:
				if(mode=='L') c=tolower(c);
				if(mode=='U') c=toupper(c);

				if( strchr(allowed_set, c) && len<maxlength ) {
					out[len]=c;
					len++; coordx++;
					printw("%c",c);
				} else {
					beep();
				}
				break;

		} /* end CASE */


	} while(c!=10&&c!=13);

	curs_set(0);
	return ( strlen(out) );
}

