#!/bin/sh

CFLAGS=-O3 -I../ucode/include/ -I/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim/include/

CFLAGS_DEBUG=-O0 -I../ucode/include/ -DDEBUG -g


include ../Makefile.include

all: driver.o
driver.o: driver.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -Rf ./driver.o
