#
# Semplice makefile per compilare i programmi necessari ..
#
inc      := /usr/include/mysql
lib      := /usr/lib64/mysql

# System environment
CC = gcc

CODEGEN =
OPTIMIZATION = -O0
# OPTIONS = -D_MG_BIG_ENDIAN_ -D_MG_NCURSES_
# OPTIONS = -DDEBUG
DEBUG = -g
PRJ=gencachemdir.c

# CFLAGS = $(CODEGEN) $(OPTIMIZATION) $(OPTIONS) $(DEBUG)
CFLAGS = $(CODEGEN) $(OPTIMIZATION) $(OPTIONS)
CPPFLAGS := -I$(inc) -D_THREAD_SAFE -D_REENTRANT

#LIB = -lncurses
#LIB = -lpcap
#LIB = 
LIB    = -L$(lib) -lmysqlclient -lz -lm -lcrypt

COMPLINK = $(CC) $(CFLAGS) $(CPPFLAGS) $(INCL) -Wall
COMPILE = $(CC) $(CFLAGS)  $(CPPFLAGS) $(INCL) -Wall -c

SOURCES = gencachemdir.c 
# OBJ = presta.o senal.o strfunz.o
OBJ = ${SOURCES:.c=.o}
PRGS = ${SOURCES:.c=}

ifneq (,$(shell grep FreeBSD /COPYRIGHT 2>/dev/null))
# FreeBSD
LDFLAGS += -pthread
else
# Assume Linux
LDLIBS += -lpthread
endif
all: gencachemdir

# presta: strfunz.o presta.o
# 	$(COMPLINK) -o $@ presta.o strfunz.o
# senal: strfunz.o senal.o
# 	$(COMPLINK) -o $@ senal.o strfunz.o

# gencachemdir: gencachemdir.o
# 	$(COMPLINK) -o $@ gencachemdir.o $(LIB)
# 

gencachemdir: gencachemdir.o strfunz.o
	$(COMPLINK) -o $@ $^ $(LIB)

gencachemdir.o: gencachemdir.c
	$(COMPILE) gencachemdir.c
# strfunz.o: strfunz.c
# 	$(COMPILE) strfunz.c

somiglia: somiglia.o strfunz.o
	$(COMPLINK) -o $@ $^

somiglia.o: somiglia.c 
	$(COMPILE) somiglia.c

strfunz.o: strfunz.c strfunz.h
	$(COMPILE) strfunz.c

clean:
	rm -f $(PRGS) $(OBJ) *~ *.o somiglia
