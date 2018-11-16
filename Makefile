#
# Makefile for 0cc
#
CC=gcc
CFLAG=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


0cc: $(OBJS)

$(OBJS): 0cc.h

test: 0cc
	./test.sh

clean:
	rm -f 0cc *.o *~ tmp*
