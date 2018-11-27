#!/bin/bash

DMCPOWER=../dmc_power

# TRACEFILE=../scatter.out

TRACEFILE=../coalesced.txt

CFILE=./config.dmc

echo "Executing : $DMCPOWER -f $TRACEFILE -C $CFILE -D"

$DMCPOWER -f $TRACEFILE -C $CFILE -D
