#!/bin/bash

DMCPOWER=../dmc_power

TRACEFILE=/home/shared/data/scatter.out

CFILE=./config.dmc

echo "Executing : $DMCPOWER -f $TRACEFILE -C $CFILE "

$DMCPOWER -f $TRACEFILE -C $CFILE
