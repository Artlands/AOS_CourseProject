#!/bin/sh
#
# GC64-DMC Software Top-level Makefile
#

include Makefile.include

SRCS:=./ucode/trace.o ./ucode/ucode.o ./driver/driver.o

all: dmc_driver
dmc_driver: driver ucode
	$(CC) $(CFLAGS) -o $@ $(SRCS)
driver:
	make -C driver
ucode:
	make -C ucode
clean:
	rm -Rf dmc_driver
	make -C driver clean
	make -C ucode clean
	make -C utils clean

.PHONY: driver ucode utils clean
