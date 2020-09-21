ifndef CC
	CC=gcc
endif
CFLAGS=-std=c99 -Wall -Wpedantic -Wextra
SRCS=main.c main2.c main3.c
OBJS=$(subst .c,.o,$(SRCS))
RM=rm -f

all: main main2 main3

foo: main.o main2.o main3.o
	$(CC) $(CFLAGS) -o main main.c main2 main2.c main3 main3.c

clean:
	$(RM) $(OBJS) main main2 main3

