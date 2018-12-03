#!/bin/sh

CFLAGS=-O3 -I../ucode/include/ -I/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim/include/

CFLAGS_DEBUG=-O0 -I../ucode/include/ -DDEBUG -g

LIBS = -L/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim -lhmcsim

include ../Makefile.include

all: driver.o
driver.o: driver.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)
clean:
	rm -Rf ./driver.o
