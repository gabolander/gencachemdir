/* INIZIO_MOD -----------------------------------------------------------------
Modulo STRFUNZ

Le funzioni appartenenti a questo modulo, sono utilizzate per la gestione
ad alto livello delle stringhe. Alcune di esse rimpiazzano le funzioni
della libreria standard del compilatore C.
------------------------------------------------------------------- FINE_MOD */
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include "strfunz.h"
// #include "ruota.h"

/* Prototype del sorgente strfunz.c */
extern int strmid (const char *, int , int , char *);
extern int strigh (char *, int , char *);
extern int strpul (char *);
extern int strpuldx (char *);
extern int strcop (char *, char *, int , int , int );
extern int strins (char *, int , char *);
extern int strdel (char *, int , int );
extern int strlow (char *);
extern int strupper (char *);
extern int strcerca (int , const char *, const char *);
extern int strfill (char *, unsigned char , int );
extern int strncpy0 (char *, const char *, int );
extern char *str_reverse (char *);
extern void str_sost (char *, char , char );
// extern int strvuota(const char *);
extern int strnvuota (const char *, int);
extern int str_shift_sx (char *, int );
extern int strip (double , int , char *);

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Estrae una sottostringa da una stringa di origine partendo dalla
          posizione pos per una lunghezza di len caratteri. La stringa di
          ritorno e' terminata da un NULL.
Utilizzo: -
Input:    str_origine - stringa da cui estrarre.
          pos         - posizione da cui iniziare ad estrarre.
          len         - numero caratteri da estrarre.
Output:   str_ritorno - sottostringa risultante.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strmid(str_origine, pos, lun, str_ritorno)
const char *str_origine;
char str_ritorno[];
int pos, lun;
{
	int i;
	for (i=0;i<lun;++i) str_ritorno[i]=str_origine[pos+i]; str_ritorno[i]='\0';
	return(0);
}


/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Estrae dalla stringa di origine, gli ultimi n_car caratteri.
          La sottostringa risultante e' terminata da un NULL.
Utilizzo: -
Input:    str_origine - stringa di origine.
          n_car       - numero caratteri da estrarre.
Output:   str_ritorno - sottostringa estratta.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strigh(str_origine, n_car, str_ritorno)
char str_origine[];
int n_car;
char str_ritorno[];
   {
   register int i;
   for (i=0;i<n_car;++i)
		str_ritorno[i] = str_origine[strlen(str_origine)-n_car+i]; 
	str_ritorno[i] = '\0';
	return(0);
   }


/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Elimina gli spazi a destra e a sinistra da una stringa.
Utilizzo: -
Input:    stringa - con spazi a sinistra e destra.
Output:   stringa - senza spazi.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strpul(stringa)
char stringa[];
   {
   register int i;
   i=strlen(stringa)-1;
   if (i<0) return(0);
   while (i>=0 && stringa[i]==' ') {stringa[i]='\0'; --i;}
   while (stringa[0]==' ') for (i=0;stringa[i]!='\0';++i) stringa[i]=stringa[i+1];
	return(0);
	}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Elimina gli spazi a destra da una stringa.
Utilizzo: -
Input:    stringa - con spazi a destra.
Output:   stringa - senza spazi a destra.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strpuldx(stringa)
char stringa[];
   {
   register int i;
   i=strlen(stringa)-1;
   if (i<0) return(0);
   while (i>=0 && stringa[i]==' ') {stringa[i]='\0'; --i;}
	return(0);
   }

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Copia nella stringa str_dest, a partire dalla posizione pos_dest,
          il contenuto della stringa str_orig a partire dalla posizione pos_orig
          per la lunghezza espressa dal parametro n_car.
Utilizzo: -
Input:    str_orig - stringa da cui copiare.
          pos_orig - posizione da cui iniziare la copia.
          pos_dest - posizione destinazione della copia.
          n_car    - numero caratteri da copiare.
Output:   str_dest - stringa contenente la zona copiata.
Ritorno:  il numero di caratteri copiati.
------------------------------------------------------------------- FINE_FUNZ */

int strcop(str_dest, str_orig, pos_dest, pos_orig, n_car)

char str_dest[];
char str_orig[];
int pos_dest;
int pos_orig;
int n_car;
{
	memcpy( str_dest+pos_dest, str_orig+pos_orig, n_car );
	return( n_car );
}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Inserisce nella stringa data alla posizione pos, la stringa str_ins.
Utilizzo: -
Input:    stringa - stringa in cui inserire la stringa ins.
          pos     - posizione da cui iniziare l'inseriemento.
          str_ins - stringa da inserire.
Output:   stringa - stringa risultante dopo l'inserimento.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strins(stringa, pos, str_ins)
char stringa[];
int pos;
char str_ins[];
   {
   register int u;
   int li,ls;

   li=strlen(str_ins);
   ls=strlen(stringa);

   for (u=0; u<=ls-pos; ++u) stringa[ls+li-u] = stringa[ls-u];
   for (u=0; u<li;      ++u) stringa[pos+u]   = str_ins[u];
	return(0);
   }

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Elimina dalla stringa di origine, a partire dalla posizione pos,
          il numero di caratteri specificato da n_car.
Utilizzo: -
Input:    stringa - stringa da cui cancellare i caratteri.
          pos     - posizione da cui cancellare i caratteri.
          n_car   - numero di caratteri da cancellare.
Output:   stringa - stringa risualtante dopo la cancellazione.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strdel(stringa, pos, n_car)
char stringa[];
int pos;
int n_car;
   {
   int i,u,ls;
   for (u=1;u<=n_car;++u)
      {
      ls=strlen(stringa);
      for (i=pos;i<ls;++i) stringa[i]=stringa[i+1];
      }
	return(0);
   }

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Converte tutti i caratteri maiuscoli di una stringa in minuscoli.
Utilizzo: -
Input:    stringa - contenente sia caratteri maiuscoli che minuscoli.
Output:   stringa - contenente unicamente caratteri minuscoli.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */
   
int strlow(stringa)
char stringa[];
{
	int i,ls;
	ls = strlen(stringa);
	for (i=0;i<ls;++i)
	{
		if (stringa[i] >='A' && stringa[i] <= 'Z') stringa[i] += ' ';
	}
	return(0);
}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Converte tutti i caratteri minuscoli di una stringa in maiuscoli.
Utilizzo: -
Input:    stringa - contenente sia caratteri maiuscoli che minuscoli.
Output:   stringa - contenente unicamente caratteri maiuscoli.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strupper(stringa)
char stringa[];
   {
   int i,ls;
   ls = strlen(stringa);
   for (i=0;i<ls;++i)
      {
      if (stringa[i] >='a' && stringa[i] <= 'z') stringa[i] -= ' ';
      }
	return(0);
   }

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Cerca una sottostringa all'interno di una stringa di origine.
Utilizzo: -
Input:    inizio    - posizione da cui iniziare la ricerca.
          str_orig  - stringa in cui ricercare.
          str_daric - stringa da ricercare.
Output:   -
Ritorno:  la posizione in cui e' stata trovata la stringa, oppure
          -1 se la stringa non e' stata trovata.
------------------------------------------------------------------- FINE_FUNZ */

int strcerca(inizio, str_orig, str_daric)

int  inizio;
const char *str_orig;
const char *str_daric;
{
  register int x,y;
  int p,l1,l2;

  y=0; 
	p=(-1);
	l1=inizio + strlen(&str_orig[inizio]);
	l2=strlen(str_daric);

  for (x=inizio; x<l1; x++)
  {
    if ( str_orig[x] != str_daric[y] )
    {
      /* riparto dalla prima posiz. successiva all'inizio del ritrovam. */
      if (p >= 0) x = p;
      p=(-1);
      y=0;
      continue;
    }
    if ( p < 0 ) p=x;
    y++;
    if ( y == l2 ) break;
  }
  if ( y < l2 ) p=(-1);
  
  return(p);
}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Riempie la stringa di origine con il carattere dato.
Utilizzo: -
Input:    str - stringa da riempiere.
          car - carattere con cui riempire la stringa.
          len - numero di caratteri con cui riempire la stringa.
Output:   str - contenente i caratteri di riempimento.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strfill(str, car, len)
char *str;
unsigned char car;
int  len;
{
	memset( str, car, len );
	str[len] = '\0';

	return(0);
}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Copia un numero di caratteri dalla stringa origine a quella di
          destinazione e chiude quest'ultima con un NULL.
          Per una spiegazione piu' dettagliata guardare la rem qui sotto
          (essa non fa altro che riportare la descrizione del funzionamento
          riportata sul manuale della funzione strncpy)
Utilizzo: -
Input:    str_orig - stringa da cui copiare i caratteri.
          num_car  - numero di caratteri da copiare.
Output:   str_dest - stringa destinazione della copia.
Ritorno:  -
------------------------------------------------------------------- FINE_FUNZ */

int strncpy0(str_dest, str_orig, num_car)
char str_dest[];
const char *str_orig;
int  num_car;
{
   if( num_car < 0 )   num_car=0;
   /*
   ** Se num_car e' piu' piccolo della lunghezza di str_orig, i caratteri
   ** compresi fra la lunghezza di str_orig e num_car rimarranno
   ** inalterati e il \0 verra' messo solo alla posizione num_car.
   **
   ** Se num_car e' maggiore della lunghezza di str_orig, i caratteri di
   ** str_dest compresi fra la lunghezza di str_orig e num_car saranno
   ** riempiti di \0.
   */
	strncpy(str_dest, str_orig, num_car);
	str_dest[num_car] = '\0';
	return(0);
}


/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Inverte la stringa passata.
Utilizzo: -
Input:    stringa - da invertire.
Output:   stringa - invertita.
Ritorno:  il puntatore alla stringa invertita.
------------------------------------------------------------------- FINE_FUNZ */

char *str_reverse(stringa)

char *stringa;
{
	char c;
	int i, j;

	for (i = 0, j = (strlen(stringa) - 1); i < j; i++, j--) 
	{
		c = stringa[i];
		stringa[i] = stringa[j];
		stringa[j] = c;
	}
	return(stringa);
}



/*
** cerca in tutta una stringa.
** dove trova il carattere daric lo sostituisce con sost.
*/

void str_sost( str, daric, sost )
char *str;
char daric, sost;
{
	int ct;

	for( ct = 0; ct< strlen(str); ct++ )
		if( str[ct] == daric )
			str[ct] = sost;
}

/*
** Testa se una stringa e' vuota:
** Sia se e' lunga 0,
** sia se e' piena di blank
*/

/* -- messa come macro define
int strvuota( str )
	const char *str;
{
	return( str==NULL ? SI : strnvuota( str, strlen(str) ) );
}
*/

int strnvuota( str, nbyte )
const char *str;
int nbyte;
{
	register int ct;
	int vuota = SI;
	
	if( str != NULL )
		for( ct = 0; ct < nbyte; ct++ )
		{
			if( str[ct] == '\0' )
				break;
			if( str[ct] != ' ' && str[ct] != '\t' )
			{
				vuota = NO;
				break;
			}
		}
				
	return( vuota );
}


/*
** Shifta a sinistra la stringa str di n caratteri ponendo a blank la parte dx
*/
int str_shift_sx( str, n )
char *str;
int n;
{
	int ret=ERRORE;
	int len_str;
	char *pstr;
	char *lstr;
	
	len_str = strlen( str );
	if( n <= len_str )
	{
		/* punta all'ennesimo carattere della stringa str */
		pstr = str+n;
		/* punta all'inizio di str */
		lstr = str;
		/* copia a partire dalla dall' ennesimo carattere sul primo */
		while( *pstr ) *(lstr++) = *(pstr++);
		/* pulizia degli ennultimi caratteri rimasti */
		while( n-- > 0 && *lstr ) *(lstr++) = ' ';
		ret = _OK;
	}
	return( ret );
}

/* INIZIO_FUNZ -----------------------------------------------------------------
Scopo:    Il contenuto di valore viene copiato nella stringa di ritorno,
          escludendo la parte decimale ed inserendo i separatori delle migliaia.
          La stringa risultante verra' allineata a destra e sara' lunga lun
          caratteri.
Utilizzo: -
Input:    valore      - numero da convertire in stringa.
          lun         - lunghezza della stringa di ritorno.
Output:   str_ritorno - stringa numerica formattata.
Ritorno:  la lunghezza della stringa str_ritorno formattata.
------------------------------------------------------------------- FINE_FUNZ */

int strip(valore, lun, str_ritorno)

double valore;
int    lun;
char   str_ritorno[];

   {
   int i,i1,l;
   char s[128];

   l=sprintf(s,"%.0f",valore);
   if (l>lun) return(0);

	sprintf(str_ritorno, "%*.*s", lun, lun, " ");
   for (i=1, i1=lun; i<=l; ++i)
		{
      --i1; str_ritorno[i1]=s[l-i];
      if (i%3==0 && i<l) { --i1; str_ritorno[i1]='.'; }
      }
   return(strlen(str_ritorno));
   }

