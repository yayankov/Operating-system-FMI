ifndef CC
	CC=gcc
endif
CFLAGS=-std=c99 -Wall -Wpedantic -Wextra
SRCS=main.c bar.c
OBJS=$(subst .c,.o,$(SRCS))
RM=rm -f

all: main bar

foo: main.o bar.o
	$(CC) $(CFLAGS) -o main main.c bar bar.c

clean:
	$(RM) $(OBJS) main bar

