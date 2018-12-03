#!/bin/sh
#
# GC64-DMC Software Top-level Makefile
#

include Makefile.include

LIBS = -L/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim -lhmcsim

SRCS:=./ucode/trace.o ./ucode/ucode.o ./driver/driver.o

all: dmc_driver
dmc_driver: driver ucode
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LIBS)
driver:
	make -C driver
ucode:
	make -C ucode
clean:
	rm -Rf dmc_driver
	make -C driver clean
	make -C ucode clean

.PHONY: driver ucode utils clean

# include Makefile.include
#
# SRCS:=./ucode/trace.o ./ucode/ucode.o ./ucode/ucode_opt.o ./driver/driver.o
#
# all: dmc_driver
# dmc_driver: driver ucode utils
# 	$(CC) $(CFLAGS) -o $@ $(SRCS)
# driver:
# 	make -C driver
# utils:
# 	make -C utils
# ucode:
# 	make -C ucode
# clean:
# 	rm -Rf dmc_driver
# 	make -C driver clean
# 	make -C ucode clean
# 	make -C utils clean
#
# .PHONY: driver ucode utils clean
