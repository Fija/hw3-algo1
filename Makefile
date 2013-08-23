CC=gcc
CFLAGS=-g -Wall -pg
LDFLAGS=-g -Wall -pg
cut:cut.o arrlib.o
.c.o:
clean:
	rm cut cut.o arrlib.o

