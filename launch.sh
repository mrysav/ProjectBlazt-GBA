#!/bin/bash

FIRST_GBA=$(ls -1 *.gba | head -n 1)
EMU_BIN="mgba-qt"

bash -c "$EMU_BIN $FIRST_GBA" 
