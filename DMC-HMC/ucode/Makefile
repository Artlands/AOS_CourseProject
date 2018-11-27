#!/bin/sh

CFLAGS := -O3 -std=c99 -I`pwd`/include

include ../Makefile.include

all: ucode.o trace.o
trace.o: trace.c
	$(CC) $(CFLAGS) -c -o $@ $<
ucode.o: ucode.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -Rf ./*.o
