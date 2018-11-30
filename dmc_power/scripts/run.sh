#!/bin/bash

DMCPOWER=../dmc_power

# TRACEFILE=../scatter.out

TRACEFILE=../dmc_scatter.out

CFILE=./config.dmc

echo "Executing : $DMCPOWER -f $TRACEFILE -C $CFILE -D"

$DMCPOWER -f $TRACEFILE -C $CFILE -D
