#ifndef ERRORE
#define ERRORE  (-1)
#endif
#ifndef NO
#define NO        0
#endif
#ifndef SI
#define SI        1
#endif
#ifndef IGNORA
#define IGNORA    2
#endif
#ifndef DEFAULT
#define DEFAULT   3
#endif
#ifndef NOBUFF
#define NOBUFF    4
#endif
#ifndef _OK
#define _OK       0
#endif




extern int strmid (const char *, int, int, char *);
extern int strigh (char *, int, char *);
extern int strpul (char *);
extern int strpuldx (char *);
extern int strcop (char *, char *, int, int, int);
extern int strins (char *, int, char *);
extern int strdel (char *, int, int);
extern int strlow (char *);
extern int strupper (char *);
extern int strcerca (int, const char *, const char *);
extern int strfill (char *, unsigned char, int);
extern int strncpy0 (char *, const char *, int);
extern char *str_reverse (char *);
extern void str_sost (char *, char, char);
extern int strnvuota (const char *, int);
extern int str_shift_sx (char *, int);
extern int strip (double, int, char *);
extern int strmid (const char *str_origine, int pos, int lun, char *str_ritorno); /* (str_origine, pos, lun, str_ritorno) const char *str_origine; int pos; int lun; char *str_ritorno; */
extern int strigh (char *str_origine, int n_car, char *str_ritorno); /* (str_origine, n_car, str_ritorno) char *str_origine; int n_car; char *str_ritorno; */
extern int strpul (char *stringa); /* (stringa) char *stringa; */
extern int strpuldx (char *stringa); /* (stringa) char *stringa; */
extern int strcop (char *str_dest, char *str_orig, int pos_dest, int pos_orig, int n_car); /* (str_dest, str_orig, pos_dest, pos_orig, n_car) char *str_dest; char *str_orig; int pos_dest; int pos_orig; int n_car; */
extern int strins (char *stringa, int pos, char *str_ins); /* (stringa, pos, str_ins) char *stringa; int pos; char *str_ins; */
extern int strdel (char *stringa, int pos, int n_car); /* (stringa, pos, n_car) char *stringa; int pos; int n_car; */
extern int strlow (char *stringa); /* (stringa) char *stringa; */
extern int strupper (char *stringa); /* (stringa) char *stringa; */
extern int strcerca (int inizio, const char *str_orig, const char *str_daric); /* (inizio, str_orig, str_daric) int inizio; const char *str_orig; const char *str_daric; */
extern int strfill (char *str, unsigned char car, int len); /* (str, car, len) char *str; unsigned char car; int len; */
extern int strncpy0 (char *str_dest, const char *str_orig, int num_car); /* (str_dest, str_orig, num_car) char *str_dest; const char *str_orig; int num_car; */
extern char *str_reverse (char *stringa); /* (stringa) char *stringa; */
extern void str_sost (char *str, char daric, char sost); /* (str, daric, sost) char *str; char daric; char sost; */
extern int strnvuota (const char *str, int nbyte); /* (str, nbyte) const char *str; int nbyte; */
extern int str_shift_sx (char *str, int n); /* (str, n) char *str; int n; */
extern int strip (double valore, int lun, char *str_ritorno); /* (valore, lun, str_ritorno) double valore; int lun; char *str_ritorno; */
