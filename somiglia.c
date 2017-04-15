/*
 * This program is copyrighted to Gabriele Zappi (C)
 * ??? 200?
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <getopt.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "strfunz.h"

#ifndef strlower
#define strlower strlow
#endif

#define MAX_BUFFER_SIZE (1024*100)
#define MAX(a,b)	(a>b)?(a):(b)
#define MIN(a,b)	(a<b)?(a):(b)
#define DEFAULT_MEM_SIZE 50000000L
#define MAX_ELEM 6
#define SECONDI(a)  (a/CLOCKS_PER_SEC)
#define MAX_ITERATIONS 20000 /* ITERAZIONI MASSIME PER DIZIONARIO */

#define TRUE 1
#define FALSE 0
#define MAXVETT 30


#define S_DIVERI 0														 
#define S_UGUALI 65535														

/* typedefs */
typedef short int sint;
typedef unsigned short int usint;
typedef unsigned int uint;
typedef char byte;
typedef unsigned char bool;
typedef unsigned char uchar;


/* Global vars */
char * P_programname;
char * P_filename=NULL;

/* prototipi di funzione */
int somiglia( const char * , const char * );

/*
 *
 *      M A I N
 *
 */
int main(int argc, char *argv[])
{
	// register int i,j,iter,volte=0;
	int ilike;
	char *r;


	if ( (r=strrchr( argv[0], '/' ))!=(char *) NULL ) {
		P_programname=r+1;
	} else {
		P_programname=argv[0];
	}

	if( argc < 3 ) {
		printf("%s vuole almeno due parametri:\n", P_programname);
		printf(" %s [parola1] [parola2]       \n", P_programname);
		printf("    Verifica la somiglianza di parola2 all'interno di parola1. \n");
		test();
		exit(1);
	}

	if( (ilike=somiglia(argv[1],argv[2])) ) {
		switch( ilike ) {
			case S_UGUALI:
				printf( "%s e %s sono uguali.\n", argv[1], argv[2] );
				break;
			default:
				printf( "%s e %s hanno qualche altro tipo di somiglianza.\n", argv[1], argv[2] );
				break;
		}
	} else {
		printf( "%s e %s non si somigliano affatto.\n", argv[1], argv[2] );
	}


	return(0);
	exit(0);
}

int somiglia( const char * stringa, const char * pattern )
{
	int j=0;
	char * p_stringa = NULL;
	char * p_pattern = NULL;
	int difflen;
	int len_stringa, len_pattern;

	if( (p_stringa=malloc( strlen(stringa) * sizeof(char) + 3 )) == NULL ) {
		printf( "Insuff. memory!\n" );
		exit(1);
	}
	if( (p_pattern=malloc( strlen(pattern) * sizeof(char) + 3 )) == NULL ) {
		printf( "Insuff. memory!\n" );
		exit(1);
	}

	strcpy(p_stringa, stringa); strpul(p_stringa); strlow(p_stringa);
	strcpy(p_pattern, pattern); strpul(p_pattern); strlow(p_pattern);

	len_stringa=strlen(p_stringa);
	len_pattern=strlen(p_pattern);
	difflen=(len_stringa-len_pattern); if( difflen<0 ) difflen*=-1;

	printf( "La differenza di lunghezza tra le due str e' : %d \n", difflen );

	if( !strcmp(p_stringa, p_pattern) )
		return S_UGUALI;

	/* Ricerca per somiglianza lettere */

	/* Ricerca per 1 lettera diversa in pattern */

	/* Ricerca per mancanza 1 lettera in pattern */

	/* Ricerca per 1 lettera in piu' in pattern */

	return( j );
}

int test()
{
	char vett[10][5]={
			{'1','2','3','4','5'},
			{'1','2','3','5',0},
			{'1','2',0,0,0},
			{'1','2','3','4','5'},
			{'1','2','3','4','5'},
			{'1','2','3',0,0},
			{'1','2','3','4','5'},
			{'1','2','3','4','5'},
			{'1','2','3','4','5'},
			{'1','2','3','4','5'}
	};
	int y,x;

	for(y=0;y<10;y++) {

		printf( "La linea %d e' lunga %d e contiene: ",y+1, sizeof(vett[y]) );
		for(x=0;x<sizeof(vett[y]);x++) {
			printf( "\'%c\' (dec:%d,hex:%x)\t",vett[y][x],vett[y][x],vett[y][x]);
		}
		putchar('\n');

	}
}
