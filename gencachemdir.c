/*
 * This program is copyrighted to Gabriele Zappi (C)
 * Nov 2004
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <getopt.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
/* #include "lc_language.h" */
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
#define MAX_FILENAME_LENGTH 1024

#define TRUE 1
#define FALSE 0
#define MAXVETT 30

#define PASSWD_MIN_LENGTH 6
#define PASSWD_MAX_LENGTH 12
#define PROGRAMNAME "genpassword"
#define AUTHOR "Gabriele Zappi"
#define AUTHOR_NICKNAME "Gabo"
#define VERSION "1.0.3rc2"
#define PROGRAM_DATE "mag 2010"

#define HOMEVMAIL "/home/vmail/"
#define DEFAULT_DOMAIN "comune.rimini.it"

/* typedefs */
typedef short int sint;
typedef unsigned short int usint;
typedef char byte;
typedef unsigned char bool;
typedef unsigned char uchar;

/* const chars */
/* const char k_passwd_chars[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]~!@#$^&*()-_=+\\|;:'\",.<>/?"; */

const char k_passwd_digit_chars[]="0123456789";
const char k_passwd_uppercase_chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char k_passwd_lowercase_chars[]="abcdefghijklmnopqrstuvwxyz";
const char k_passwd_symbol_chars[]="[]~!@#$^&*()-_=+\\|;:'\",.<>/?";

/* Global vars */
usint Passwd_size = 0;
sint Digit_ratio = -1, Uppercase_ratio = -1, Symbol_ratio = -1;
uchar Flag_digit_type = ' ', Flag_uppercase_type = ' ', Flag_symbol_type = ' ';
bool Flag_no_digit = FALSE;
bool Flag_no_uppercase = FALSE;
bool Flag_no_symbol = FALSE;
bool Flag_short = FALSE;
bool Flag_db = FALSE;
bool Flag_exact_match = FALSE;

MYSQL mysql;

char * P_passwd_chars;
int I_passwd_char_length=0;
char * P_programname;
char * P_filename=NULL;
char * P_sql_args;
char ** Pa_sql_args;

/* prototipi di funzione */
int	genera_password(char * ,int );
int	estrai_lunghezza_password(void);
void version(void);
void usage(void);
char ** break_args( const char *, const char );
void gestisci_mysql_error( int );
int esamina_maildir_utente( const char *, long *, long *, int *);
int aggiorna_cache_quota( const char *, long, long, int );
int puldim( char *, int );

/*
 *
 *      M A I N
 *
 */
int main(int argc, char *argv[])
{
	register int i,j,iter,volte=0;
	int primo_parametro = 0;
	unsigned int rand_seed;
	clock_t clk;
	char c;
	struct tms *tempo;
	int error=0;
	char *r;
	FILE *f_out;
	char password[91];
	/* int digit_optind = 0; */

	/* Variabili per DB mysql */
	char query_string[1001],tabella[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	// MYSQL_ROW end_row;
	unsigned int num_fields;
	unsigned int num_rows;
	int query;
	unsigned long *lengths;

	DIR *dirp;
	struct dirent * structfilep;
	char directorybase[1025];
	char nome_file[1025];
	char nome_dir[1025];
	
	
	if ( (r=strrchr( argv[0], '/' ))!=(char *) NULL ) {
		P_programname=r+1;
	} else {
		P_programname=argv[0];
	}

	/* gestione dei parametri (getopt) */
	while (1) {
#if 0
		 int this_option_optind = optind ? optind : 1;
#endif
		 int option_index = 0;
		 static struct option long_options[] = {
				 // {"verbose", 0, 0, 0},
				 {"version", 0, 0, 'v'},
				 {"help", 0, 0, 'h'},
				 /* {"create", 1, 0, 'c'}, */
				 // Ratio for numbers in password (no short flag )
				 {"digit-ratio", 1, 0, 0},
				 // Ratio for Uppercase chars in password (no short flag )
				 {"uppercase-ratio", 1, 0, 0},
				 // Ratio for Symbols in password  (no short flag )
				 {"symbol-ratio", 1, 0, 0},
				 // No Symbols in password (short: -n )
				 {"no-symbol", 0, 0, 'n'},
				 {"no-symbols", 0, 0, 'n'},
				 // No digit in password (short: -x )
				 {"no-digit", 0, 0, 'x'},
				 {"no-digits", 0, 0, 'x'},
				 // No uppercase in password (short: -l )
				 {"no-upper", 0, 0, 'l'},
				 {"no-uppercase", 0, 0, 'l'},
				 {"no-uppercase-chars", 0, 0, 'l'},
				 // Password length parameter(s) (short: -s )
				 {"size", 1, 0, 's'},
				 {"password-size", 1, 0, 's'},
				 {"password-length", 1, 0, 's'},
				 // Output file parameter (short: -o )
				 {"file", 1, 0, 'o'},
				 {"output-file", 1, 0, 'o'},
				 // Nuovo: GABO il 05-06-2006 - Con dizionario su db mysql (short: -d)
				 {"with-dict",1,0,'d'},
				 {"with-dictionary",1,0,'d'},
				 {"use-dict",1,0,'d'},
				 {"use-dictionary",1,0,'d'},
				 // Nuovo: GABO il 06-06-2006 - Match esatto! (no short flag)
				 {"exact-match",0,0,0},
				 // short output (short: -S)
				 {"short", 0, 0, 'S'},
				 /* {"optional", 2, 0, 0}, */
				 {0, 0, 0, 0}
		 };

		 c = getopt_long (argc, argv, "?Shvnxlo:s:d:",
							long_options, &option_index);
		 if (c == -1)
				 break;

		 switch (c) {
		 case 0:
#ifdef DEBUG
				 printf ("option %s", long_options[option_index].name);
				 if (optarg)
						 printf (" with arg %s", optarg);
				 printf ("\n");
#endif
				 if(!strcmp(long_options[option_index].name,"symbol-ratio"))
				 {
					 Flag_symbol_type=(strchr(optarg,'%')) ?
						 								'%':' ';
					 Symbol_ratio=atoi(optarg);
#ifdef DEBUG
					 printf("Opzione %s: ratio=%d, tipo=%c\n",
							 long_options[option_index].name, Symbol_ratio, Flag_symbol_type);
#endif

				 }
				 if(!strcmp(long_options[option_index].name,"digit-ratio"))
				 {
					 Flag_digit_type=(strchr(optarg,'%')) ?
						 								'%':' ';
					 Digit_ratio=atoi(optarg);
#ifdef DEBUG
					 printf("Opzione %s: ratio=%d, tipo=%c\n",
							 long_options[option_index].name, Digit_ratio, Flag_digit_type);
#endif

				 }
				 if(!strcmp(long_options[option_index].name,"uppercase-ratio"))
				 {
					 Flag_uppercase_type=(strchr(optarg,'%')) ?
						 								'%':' ';
					 Uppercase_ratio=atoi(optarg);
#ifdef DEBUG
					 printf("Opzione %s: ratio=%d, tipo=%c\n",
							 long_options[option_index].name, Uppercase_ratio, Flag_uppercase_type);
#endif

				 }
				 if(!strcmp(long_options[option_index].name,"exact-match"))
				 {
					 Flag_exact_match=TRUE;
#ifdef DEBUG
					 printf("Opzione %s attivata\n", long_options[option_index].name);
#endif

				 }
				 break;
#if 0
		 case '0':
		 case '1':
		 case '2':
				 if (digit_optind != 0 && digit_optind != this_option_optind)
					 printf ("digits occur in two different argv-elements.\n");
				 digit_optind = this_option_optind;
				 printf ("option %c\n", c);
				 break;
#endif

		 case 'n':
#ifdef DEBUG
				 printf ("option n\n");
#endif
				 Flag_no_symbol=TRUE;
				 break;

		 case 'l':
#ifdef DEBUG
				 printf ("option l\n");
#endif
				 Flag_no_uppercase=TRUE;
				 break;

		 case 'x':
#ifdef DEBUG
				 printf ("option x\n");
#endif
				 Flag_no_digit=TRUE;
				 break;

		 case 'o':
#ifdef DEBUG
				 printf ("option o with value `%s'\n", optarg);
#endif
				 P_filename=optarg;
				 break;

		 case 's':
#ifdef DEBUG
				 printf ("option s with value `%s'\n", optarg);
#endif
				 Passwd_size=atoi(optarg);
				 if( Passwd_size<PASSWD_MIN_LENGTH ) {
					 printf ("Password must be at least %d chars long.\n", PASSWD_MIN_LENGTH);
					 error=1;
				 }
				 if( Passwd_size>PASSWD_MAX_LENGTH ) {
					 printf ("Password length must be max %d chars long.\n", PASSWD_MAX_LENGTH);
					 error=2;
				 }
				 break;

		 // Nuovo: GABO il 05-06-2006
		 case 'd':
#ifdef DEBUG
				 printf ("option d with value `%s'\n", optarg);
#endif
				 P_sql_args=optarg;
				 if( strlen( P_sql_args ) < 4 ) {
					 printf ("Invalid SQL argument for Dictionary.\n");
					 error=1;
				 }
				 Pa_sql_args = break_args( P_sql_args, ':' );

#ifdef DEBUG
				 for(i=0;i<5;i++)
					printf("%s\n",Pa_sql_args[i]);
#endif

				 /* ora provo ad aprire il DB e fare una query */
					mysql_init(&mysql);
					mysql_options(&mysql,MYSQL_OPT_COMPRESS,0);
					// mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"odbc");
					if (!mysql_real_connect(&mysql,
											Pa_sql_args[0],Pa_sql_args[1],Pa_sql_args[2],Pa_sql_args[3],
																0,NULL,0))
					{
								fprintf(stderr, "Failed to connect to database: Error: %s\n",
															mysql_error(&mysql));
								error=1;
					}
					else
						Flag_db = TRUE;

				 break;

		 case 'S':
#ifdef DEBUG
				 printf ("option S\n");
#endif
				 Flag_short=TRUE;
				 break;

		 case 'v':
				 version();
				 error=999;
				 break;

		 case 'h':
				 version();
				 usage();
				 error=999;
				 break;

		 case '?':
				 error=3;
				 printf ("L'opzione vuole un argomento\n");
				 break;

		 default:
				 printf ("?? getopt returned character code 0%o ??\n", c);
		 }
	}

	if( error )
		exit(error>=900?0:error);

	/* Altri parametri */
	if (optind < argc) {
#ifdef DEBUG
		 printf ("non-option ARGV-elements: ");
#endif
		 while (optind < argc) {
#ifdef DEBUG
				 printf ("%d:%s ", optind, argv[optind]);
#endif
				 if(!primo_parametro) primo_parametro=optind;
				 optind++;
		 }
			 printf ("\n");
	 }

	/*  Valutazione su struttura e lunghezza range caratteri che formeranno
	 *  la password. L'eventuale incidenza (ratio) di ogni tipo di caratteri,
	 *  verra' valutata in seguito, durante la generazione della password 
	 */
	if( Flag_no_digit ) Digit_ratio = 0;
	if( Flag_no_uppercase ) Uppercase_ratio = 0;
	if( Flag_no_symbol ) Symbol_ratio = 0;

	if( Flag_db ) {
		Symbol_ratio = 0;
		if( Digit_ratio >1 ) Digit_ratio = 1;
	}
	
	I_passwd_char_length=strlen(k_passwd_lowercase_chars);
	if(Uppercase_ratio)
		I_passwd_char_length+=strlen(k_passwd_uppercase_chars);
	if(Digit_ratio)
		I_passwd_char_length+=strlen(k_passwd_digit_chars);
	if(Symbol_ratio)
		I_passwd_char_length+=strlen(k_passwd_symbol_chars);

	if( !(P_passwd_chars=(char *)malloc(I_passwd_char_length+1)) ) {
		printf( "ERR! Out of mem error!\n\n" );
		return(-1);
	}

	strcpy(P_passwd_chars, k_passwd_lowercase_chars);
	if(Uppercase_ratio)
		strcat(P_passwd_chars, k_passwd_uppercase_chars);
	if(Digit_ratio)
		strcat(P_passwd_chars, k_passwd_digit_chars);
	if(Symbol_ratio)
		strcat(P_passwd_chars, k_passwd_symbol_chars);

#ifdef DEBUG
	printf("I caratteri delle pwd sono i seguenti:\n%s\nLunghi: %d carat\n",P_passwd_chars,I_passwd_char_length);
#endif

	/* Creazione file di uscita (se richiesto) */
	if(P_filename)
		f_out=fopen(P_filename,"w+");

	/* clk = clock(); */
	tempo = malloc( sizeof(struct tms) );
	clk = times( tempo );

	rand_seed = (unsigned int)((long)clk % 32768);


#if 0	/* debug */ 
	printf( "Il seme random e' %d\n\n", rand_seed ); 
#endif
	/* debug */
	srand( rand_seed );
	
	if( primo_parametro )
		volte=atoi( argv[primo_parametro] );

	if( volte<1 ) volte=1;
	
	/*
	**
	**  INIZIO PROGRAMMA - CARICARE STRUTTURA DIRECTORY /home/vmail/{dominio}/
	**
	*/
	

#if 0		  
		  sprintf(query_string, "SELECT * FROM quota_cache WHERE matricola='CRAL' or matricola='M00122';" );
		  if ((query=mysql_real_query(&mysql,query_string,strlen(query_string))))
		  {
				  // error
				  gestisci_mysql_error( query );
		  }
		  else // query succeeded, process any data returned by it
		  {
				  result = mysql_store_result(&mysql);
				  num_rows = 0;
				  if (result)  // there are rows
				  {
						  num_fields = mysql_num_fields(result);
						  num_rows = mysql_num_rows(result);
#ifdef DEBUG
						  printf("Ok: there are %d fields!\n", num_fields );
						  printf("Ok: there are %d rows!\n", num_rows );
#endif

						  /* while ((row = mysql_fetch_row(result))) */
						  /* A me basta l'eventuale prima riga */
						  while ((row = mysql_fetch_row(result)))
						  {
							  for(j = 0; j < num_fields; j++)
							  {
									   lengths = mysql_fetch_lengths(result);
#ifdef DEBUG
									   printf("Column %u is %lu bytes and keeps [%s].\n", j, lengths[j], row[j] ? row[j] : "NULL" );
									   // printf("[%s]\t", row[i] ? row[i] : "NULL" );
#endif
							  }
#ifdef DEBUG
							  putchar('\n');
							  printf(" Password now is = \"%s\"\t", password );
#endif
						  }
#ifdef DEBUG
						  putchar('\n'); putchar('\n');
#endif

				  }
				  else  // mysql_store_result() returned nothing; should it have?
				  {
						  if(mysql_field_count(&mysql) == 0)
						  {
								  // query does not return data
								  // (it was not a SELECT)
								  num_rows = mysql_affected_rows(&mysql);
							  printf("Not a query: %d rows have been affected!\n", num_rows );
						  }
						  else // mysql_store_result() should have returned data
						  {
								  fprintf(stderr, "Error: %s\n", mysql_error(&mysql));
						  }
				  }
				  /*
				   * E' importantissimo chiamare mysql_free_result(MYSQL_RES *result) alla
				   * fine dopo aver utilizzato mysql_store_result(), altrimenti la memoria
				   * si gonfia ad ogni chiamata FINO A SCOPPIARE !!!!!!!!!
				   */
				  mysql_free_result(result);

	  } // END else if ((query=mysql_real_query(&mysql,query_string,strlen(query_string))))
	} // END else if (!mysql_real_connect ...)
	
#endif	
	
	
	
	
	sprintf(directorybase,"%s%s",HOMEVMAIL,DEFAULT_DOMAIN);

	if( access( directorybase, 06 ) )   {
	  fprintf(stderr, "Can't access directory %s - quit.\n", directorybase);
	  return(1);
	}
	P_sql_args=(char *)malloc(1025 * sizeof(unsigned char));
	
	strncpy0(P_sql_args,
			 "localhost:postfix:postfixpw:postfix", 1024);
	
	
	Pa_sql_args = break_args( P_sql_args, ':' );

	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_OPT_COMPRESS,0);
	// mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"odbc");
	if (!mysql_real_connect(&mysql,
							Pa_sql_args[0],Pa_sql_args[1],Pa_sql_args[2],Pa_sql_args[3],
												0,NULL,0))
	{
				fprintf(stderr, "Failed to connect to database: Error: %s\n",
											mysql_error(&mysql));
				return 1;
	}

	
	sprintf(query_string, "TRUNCATE TABLE quota_cache;" );
	query=mysql_real_query(&mysql,query_string,strlen(query_string));
	
	
	dirp=opendir( directorybase );
	

	while( (structfilep = (struct dirent *) readdir(dirp)) != (struct dirent *) NULL )
	{
	  int ret=0;
	  char nome_utente[1025];
	  long l_quotak=0L;
	  long l_usedk=0L;
	  int  perc=0;
	  
	  strncpy0(nome_dir, structfilep->d_name,1024);
	  strcpy(nome_utente, nome_dir);
	  strupper(nome_utente);

	  if( !strcmp(nome_dir, ".") || !strcmp(nome_dir, "..") )
			  continue;
	  
	  if( structfilep->d_type != DT_DIR )
			  continue;
	  
	  ret=esamina_maildir_utente( nome_dir, &l_quotak, &l_usedk, &perc);
#ifdef DEBUG    
           fprintf(stderr, "Per utente \"%s\" QuotaK: %ld, Usati: %ld, Perc: %d%%\n", nome_utente, l_quotak, l_usedk, perc );
#endif          
	  ret=aggiorna_cache_quota( nome_utente, l_quotak, l_usedk, perc );

	  // fprintf(stdout,"File/Dir trovato=%s (tipo=%d)\n",nome_file,structfilep->d_type); // GaboDebug
	}
  
	closedir(dirp);
	
	
	
	
	// Pulizia memoria
	for(i=0;i<5;i++)
		free(Pa_sql_args[i]);
	free(Pa_sql_args);
	
	// Chiusura Mysql
	mysql_close(&mysql);
	
	
	
	
	exit(0);
	/*
	**
	**  FINE PROGRAMMA - CARICARE STRUTTURA DIRECTORY /home/vmail/{dominio}/ [ -- CANCELLARE TUTTO IL RESTO CHE NON SERVE -- ]
	**
	*/
	
	
	
	
	if( Flag_db )
		strcpy(tabella, (strlen(Pa_sql_args[4])==0)? "dictit" : Pa_sql_args[4] );

	for(i=0;i<volte;i++) {
		int passwd_length;

		if( Flag_db ) {
		 iter=0; result=NULL;
     while(iter < MAX_ITERATIONS) {
			/* CON UNO DIZIONARIO SU DB */

			strcpy(password,"");
			passwd_length=(Passwd_size)?Passwd_size:estrai_lunghezza_password();
			if( genera_password(password,passwd_length) < 0 ) {
				exit(-1);
			}

			if( Flag_exact_match )
				sprintf(query_string, "SELECT *,soundex('%s') from %s where word='%s'", password, tabella, password );
			else
				sprintf(query_string, "SELECT *,soundex('%s') from %s where sound=soundex('%s')", password, tabella, password );
			if ((query=mysql_real_query(&mysql,query_string,strlen(query_string))))
			{
					// error
					gestisci_mysql_error( query );
					break;
			}
			else // query succeeded, process any data returned by it
			{
					result = mysql_store_result(&mysql);
					num_rows = 0;
					if (result)  // there are rows
					{
							num_fields = mysql_num_fields(result);
							num_rows = mysql_num_rows(result);
#if 0
							printf("Ok: there are %d fields!\n", num_fields );
							printf("Ok: there are %d rows!\n", num_rows );
#endif

							/* while ((row = mysql_fetch_row(result))) */
							/* A me basta l'eventuale prima riga */
							if ((row = mysql_fetch_row(result)))
							{
								for(j = 0; j < num_fields; j++)
								{
										 lengths = mysql_fetch_lengths(result);
#ifdef DEBUG
										 printf("Column %u is %lu bytes and keeps [%s].\n", j, lengths[j], row[j] ? row[j] : "NULL" );
										 // printf("[%s]\t", row[i] ? row[i] : "NULL" );
#endif
								}
#ifdef DEBUG
								putchar('\n');
								printf(" Password now is = \"%s\"\t", password );
#endif
							}
#ifdef DEBUG
							putchar('\n'); putchar('\n');
#endif

					}
					else  // mysql_store_result() returned nothing; should it have?
					{
							if(mysql_field_count(&mysql) == 0)
							{
									// query does not return data
									// (it was not a SELECT)
									num_rows = mysql_affected_rows(&mysql);
								printf("Not a query: %d rows have been affected!\n", num_rows );
							}
							else // mysql_store_result() should have returned data
							{
									fprintf(stderr, "Error: %s\n", mysql_error(&mysql));
							}
							continue;
					}
					/*
					 * E' importantissimo chiamare mysql_free_result(MYSQL_RES *result) alla
					 * fine dopo aver utilizzato mysql_store_result(), altrimenti la memoria
					 * si gonfia ad ogni chiamata FINO A SCOPPIARE !!!!!!!!!
					 */
					mysql_free_result(result);

					if( num_rows ) {
						if(Flag_short)
							printf( "%s\n", password );
						else
							printf( "%6d) La password generata e' \"%s\" [lunga %d] ed assomiglia a \"%s\" (%d' iterazione)\n", i+1, password, strlen(password),row[1],iter+1 );

						if(P_filename)
							fprintf(f_out,"%s\n",password);
						break;
					}

			}

			iter++;
		 } /* END while(iter < MAX_ITERATIONS) */

		 if(iter>=MAX_ITERATIONS) {
			 	printf( "%d iterations done with no match in dictionary!\n", MAX_ITERATIONS );
				/* c = getchar(); */
		 }

		} else {
			/* SENZA UNO DIZIONARIO SU DB */

			strcpy(password,"");
			passwd_length=(Passwd_size)?Passwd_size:estrai_lunghezza_password();
			if( genera_password(password,passwd_length) < 0 ) {
				exit(-1);
			}

			if(Flag_short)
				printf( "%s\n", password );
			else
				printf( "%6d) La password generata e' %s [lunga %d]\n", i+1, password, strlen(password) );

			if(P_filename)
				fprintf(f_out,"%s\n",password);

		}
	}

	if(P_filename)
		fclose(f_out);

	if(Flag_db) {
		for(i=0;i<5;i++)
			free(Pa_sql_args[i]);
		free(Pa_sql_args);
		mysql_close(&mysql);
	}

	return(0);
	exit(0);
}

int tira_numero_random( int finoa )
{
	int j;

	j=1+(int) ((float)finoa*rand()/(RAND_MAX+1.0));

	return( j );
}

int	genera_password(char * password,int lunghezza)
{
// const int k_passwd_chars[]="01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]~!@#$^&*()-_=+\\|;:'\",.<>/?";
	char * passtemp;
	int i,pos,range;
	sint digit_max = -1, uppercase_max = -1, symbol_max = -1;
	sint num_digit=0, num_symbol=0, num_uppercase=0;


	if( !(passtemp=(char *)malloc(lunghezza*2+1)) ) {
		printf( "ERR! Out of mem error!\n\n" );
		return(-1);
	}

// sint Digit_ratio = -1, Uppercase_ratio = -1, Symbol_ratio = -1;
// uchar Flag_digit_type = ' ', Flag_uppercase_type = ' ', Flag_symbol_type = ' ';
	/* calcolo dell'incidienza di ogni tipo di carattere */
	if( Flag_symbol_type == '%' ) {
		symbol_max = (int)((float)lunghezza*(float)Symbol_ratio/100.0+.499);
	} else {
		symbol_max = Symbol_ratio;
	}
	if( Flag_digit_type == '%' ) {
		digit_max = (int)((float)lunghezza*(float)Digit_ratio/100.0+.499);
	} else {
		digit_max = Digit_ratio;
	}
	if( Flag_uppercase_type == '%' ) {
		uppercase_max = (int)((float)lunghezza*(float)Uppercase_ratio/100.0+.499);
	} else {
		uppercase_max = Uppercase_ratio;
	}
#ifdef DEBUG
	printf("Symbol_ratio = %d\n",Symbol_ratio);
	printf("numero massimo di simboli su una lunghezza di %d = %d\n",lunghezza,symbol_max);
	printf("Digit_ratio = %d\n",Digit_ratio);
	printf("numero massimo di numeri su una lunghezza di %d = %d\n",lunghezza,digit_max);
	printf("Uppercase_ratio = %d\n",Uppercase_ratio);
	printf("numero massimo di maiuscole su una lunghezza di %d = %d\n",lunghezza,uppercase_max);
#endif
	

	range=strlen(P_passwd_chars);
	for(i=0;i<lunghezza;i++) {
		int carattere;
		pos=tira_numero_random( range - 1 );
		carattere=P_passwd_chars[pos];
//const char k_passwd_digit_chars[]="0123456789";
		if( strchr(k_passwd_symbol_chars,carattere) )
		{
			if( symbol_max>(-1) && (num_symbol+1)>symbol_max )
			{
				// scarto il carattere
				i--;
				continue;
			}
			else
				num_symbol++;
		}
		if( strchr(k_passwd_digit_chars,carattere) )
		{
			if( digit_max>(-1) && (num_digit+1)>digit_max )
			{
				// scarto il carattere
				i--;
				continue;
			}
			else
				num_digit++;
		}
		if( strchr(k_passwd_uppercase_chars,carattere) )
		{
			if( uppercase_max>(-1) && (num_uppercase+1)>uppercase_max )
			{
				// scarto il carattere
				i--;
				continue;
			}
			else
				num_uppercase++;
		}

		passtemp[i]=carattere;
	}
	passtemp[i]='\0';
	
	strcpy(password, passtemp);
	free(passtemp);

	return( strlen(password) );
}


int	estrai_lunghezza_password()
{
	int lungh=0;
	int range=0;

	range=PASSWD_MAX_LENGTH - PASSWD_MIN_LENGTH + 1;
	lungh=PASSWD_MIN_LENGTH + tira_numero_random( range ) - 1;

	return lungh;
}

void version()
{
	 printf("%s version %s\nCopyright %s", PROGRAMNAME, VERSION, AUTHOR);
#ifdef AUTHOR_NICKNAME
	 printf(" (aka `%s')", AUTHOR_NICKNAME);
#endif
#ifdef PROGRAM_DATE
	 printf(" - %s", PROGRAM_DATE);
#endif
	 putchar('\n'); putchar('\n');
}

char ** break_args( const char *argument, const char separator) 
{
	char **parts;
	char *str1,*token;
	char sep[2];
	register int i;

	parts=malloc(5*sizeof(char *));
	if(!parts) return NULL;

	/* Allocazione */
	for(i=0;i<5;i++){
		parts[i] = (char *) malloc(1000);
		parts[i][0]='\0';
	}

	sep[0]=separator; sep[1]=0;

	/* spezzetto i token */
	for(i=0,str1=argument;i<5;i++,str1=NULL){
		token=strtok(str1,sep);
		if(token==NULL) break;
		strcpy(parts[i],token);
	}

#if 0
	strcpy(parts[0],"Uno");
	strcpy(parts[1],"Due");
	strcpy(parts[2],"Tre");
	strcpy(parts[3],"Quattro");
	strcpy(parts[4],"Cinque");
#endif

	return parts;
}

void usage()
{

	printf("Uso: %s [OPZIONE]... [NUMERO_DI_PASSWORD]...\n",P_programname);
	putchar('\n');
	printf("Esempi:\n");
	printf("  %s  # visualizza una password random compresa tra %d e %d caratteri.\n",P_programname, PASSWD_MIN_LENGTH, PASSWD_MAX_LENGTH);
	printf("  %s --size=8 20  # Elenca 20 password random lunghe 8 caratteri.\n", P_programname);
	printf("  %s -s 10 -o pass.out 50  # Elenca 50 password random lunghe 10\n", P_programname);
	printf("                           # caratteri, e le scrive anche nel file pass.out.\n");
	putchar('\n');

	printf("Se una opzione lunga indica un argomento come obbligatorio, allora lo � anche\n");
	printf("per l'opzione corta equivalente.  Lo stesso vale per gli argomenti opzionali.\n");
	putchar('\n');

	printf("Opzioni principali:\n");
	printf("  -s, --size=SIZE         Crea le password di lungezza fissa specificata da\n");
	printf("                          SIZE. in assenza di questo parametro, la lunghezza\n");
	printf("                          della/delle password e variabile tra %d e %d.\n",PASSWD_MIN_LENGTH, PASSWD_MAX_LENGTH);
	printf("  -S, --short             Mostra l'output in formato corto (solo passwd)\n");
	printf("  -n, --no-symbols        esclude i symboli (:;,[]...) dai caratteri delle \n");
	printf("                          password generate.   \n");
	printf("  -l, --no-uppercase      esclude le maiuscole dai caratteri delle password\n");
	printf("                          password.   \n");
	printf("  -x, --no-digits         esclude le cifre numeriche dai caratteri delle \n");
	printf("                          password generate.   \n");
	printf("  -v, --version           indica la versione e l'autore del programma. \n");
	printf("  -h, --help              mostra questa schermata di aiuto. \n");


	putchar('\n');

	printf("Gestione delle percentuali dei tipi di caratteri che compongono le password:\n");
	printf("      --digit-ratio=NUM      indica il num. massimo delle cifre numeriche che\n");
	printf("                             andranno a comporre una password. \n");
	printf("                             Se NUM esprime un numero, e' il numero MASSIMO\n");
	printf("                             di cifre numeriche consentite nella generazione\n");
	printf("                             di una password. \n");
	printf("                             Se NUM esprime un valore percentuale (ha il carat-\n");
	printf("                             tere `%%'), indica il rapporto massimo di   \n");
	printf("                             cifre numeriche consentite nella generazione \n");
	printf("                             di una password (in rapporto alla sua lunghezza).\n");
	printf("      --uppercase-ratio=NUM  indica il num. massimo delle lettere maiscole\n");
	printf("                             che andranno a comporre una password.\n");
	printf("                             Il comportamento di NUM e' analogo al medesimo\n");
	printf("                             dell'opzione --digit-ratio.\n");
	printf("      --symbol-ratio=NUM     indica il num. massimo dei simboli ([]=+_:'\\...)\n");
	printf("                             che andranno a comporre una password.\n");
	printf("                             Il comportamento di NUM e' analogo al medesimo\n");
	printf("                             dell'opzione --digit-ratio.\n");

	printf("\n\n");
	
}

void gestisci_mysql_error( int query )
{
	// error
	printf( "Query failed !\n");
	printf( "Error is : (%s) \n ",mysql_error(&mysql) );
	printf( "Error is : (%d) : ",query );
	switch(query)
	{
		case CR_COMMANDS_OUT_OF_SYNC:
				printf( "Commands were executed in an improper order. \n" );
				break;
		case CR_SERVER_GONE_ERROR:
				printf( "The MySQL server has gone away. \n" );
				break;
		case CR_SERVER_LOST:
				printf( "The connection to the server was lost during the query. \n" );
				break;
		case CR_UNKNOWN_ERROR:
				printf( "An unknown error occurred. \n" );
				break;
		default:
				printf( "I don't know how to interpret this damn error!. \n" );
				break;
	}
}

int esamina_maildir_utente( const char * dirute, long * quota, long * used, int * perc)
{
  char maildir_path[MAX_FILENAME_LENGTH];
  char zentrata[MAX_BUFFER_SIZE];
  int count;
  FILE *fin;
  
  sprintf(maildir_path,"%s/%s/%s/Maildir/maildirsize",HOMEVMAIL,DEFAULT_DOMAIN,dirute);
  
  if( access( maildir_path, R_OK ) ) {
	fprintf(stderr, "Errore: non posso accedere in lettura al file %s\n",maildir_path);
	return 1;
  }
  
  if ((fin=fopen(maildir_path,"r")) == NULL)  /* apertura file in lettura */
  {
		/* Se la fopen ritorna NULL, significa che l'apertura del file
		sorgente e fallita,  quindi ... */
		fprintf(stderr,"ERRORE: Non posso aprire il file in input \"%s\" \n",maildir_path);
		fclose (fin);
		return (-1);    /* ...ed esce */
  }

  count=0;
  *quota=*used=0L;
  *perc=0;
  while ( !feof(fin) )
  {
		memset(zentrata, 0, sizeof( zentrata ) );
		fgets(zentrata, 590, fin );
		puldim( zentrata, 590 ); strpul(zentrata);
		
		if( feof(fin) )
        {
#if 0
           fprintf(stderr, "Fine file.\n Conenuto ultima riga: \"%s\"\n", zentrata );
#endif          
           break;
        }
		++count;
#if 0
        fprintf(stdout, "File: %s Riga %d: Conenuto riga: \"%s\"\n", maildir_path, count, zentrata );
#endif          

		if( count==1 ) {
		  *quota=atol(zentrata);
		} else {
		  *used+=atol(zentrata);
		}


  }
  
  *perc=(*quota==0L) ? 0 : (int)((*used*100L) / *quota);
  
  /* Torno i dati in K */
  *used/=1024L;
  *quota/=1024L;

  fclose(fin);
  return 0;
}

int aggiorna_cache_quota( const char * utente, long quota, long used, int perc )
{
  int ret=0;
  int query;
  char query_string[3001];
  
  sprintf(query_string, "INSERT INTO quota_cache \
							(matricola,quotamax,quotaused,perc,lastdate_cached) \
							VALUES \
							('%s',%ld,%ld,%d,now());", utente, quota, used, perc );
  if ((query=mysql_real_query(&mysql,query_string,strlen(query_string))))
  {
		  // error
		  gestisci_mysql_error( query );
		  ret=1;
  }
  
  return ret;
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
