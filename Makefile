#
# Makefile for 0cc
#
CC=gcc
CFLAGS=-Wall -std=c11 -DCC0_DEBUG
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


0cc: $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJS): 0cc.h

test: 0cc
	./test.sh

clean:
	rm -f 0cc *.o *~ tmp*
