#!/bin/bash

DMCPOWER=../dmc_power

# TRACEFILE=../scatter.out

TRACEFILE=/home/jieli/DMC-HMC/dmc_xhpcg.out

CFILE=./config.dmc

LD_LIBRARY_PATH=/home/jieli/gc64-hmcsim

export LD_LIBRARY_PATH

echo "Executing : $DMCPOWER -f $TRACEFILE -C $CFILE -D"

$DMCPOWER -f $TRACEFILE -C $CFILE -D
