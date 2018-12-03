#!/bin/bash

DMCPOWER=../dmc_power

TRACEFILE=../scatter.out

CFILE=./config.dmc

LD_LIBRARY_PATH=/Users/Artlands/Desktop/AdvancedOS/projects/gc64-hmcsim

export LD_LIBRARY_PATH

echo "Executing : $DMCPOWER -f $TRACEFILE -C $CFILE "

$DMCPOWER -f $TRACEFILE -C $CFILE
