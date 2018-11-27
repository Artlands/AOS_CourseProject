#!/bin/sh
#
# HMCSIM TEST APPLICATIONS
#
# TEST: DMC POWER
# ---------------------------------------
CFLAGS = -I/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim/include/ '-Wl,-rpath,/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim/'

CC = gcc

LIBS = -lhmcsim

SRC=./src

all: dmc_power
$(SRC)/dmc_power.o: $(SRC)/dmc_power.c
	$(CC) $(CFLAGS) -c -o $@ $<

dmc_power: $(SRC)/dmc_power.o
	$(CC) $(CFLAGS) -o $@ $(SRC)/dmc_power.c $(LIBS)

cleanout:
	rm -Rf ./scripts/*.out ./scripts/*.tec
clean:
	rm -Rf ./dmc_power ./src/*.o
