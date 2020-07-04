#!/bin/bash

if [ "$1" == '-d' ]; then
    DBG="--gdb"
fi

ROMNAME="ProjectBlazt-GBA.gba"

EMU_BIN="mgba-qt"

bash -c "$EMU_BIN $ROMNAME $DBG"
