CC=gcc
CFLAGS=-g -Wall -O3 -pg
LDFLAGS=-g -Wall -O3 -pg
cut:cut.o arrlib.o
.c.o:
clean:
	rm cut cut.o arrlib.o

