#!/bin/sh

CFLAGS := -O0

include ../Makefile.include

all: enablemem disablemem helloworld
enablemem: enablemem.c
	$(CC_LINUX) $(CFLAGS) -o $@ $<
disablemem: disablemem.c
	$(CC_LINUX) $(CFLAGS) -o $@ $<
helloworld: helloworld.c
	$(CC_LINUX) $(CFLAGS) -o $@ $<
clean:
	rm -Rf ./enablemem ./disablemem ./helloworld
