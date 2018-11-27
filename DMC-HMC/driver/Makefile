#!/bin/sh

CFLAGS=-O3 -I../ucode/include/
CFLAGS_DEBUG=-O0 -I../ucode/include/ -DDEBUG -g

include ../Makefile.include

all: driver.o
driver.o: driver.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -Rf ./driver.o
