#!/bin/bash

function strip_timestamp() {
    FILE="$(grep -vE '^\/\/\sTime' "$1")"
    echo "$FILE" > "$1"
}

# Convert character sprites
echo " - dangerous_dave.png"
grit dangerous_dave.png -ftc -gt -gB8 -Mw2 -Mh2
strip_timestamp "dangerous_dave.h"
strip_timestamp "dangerous_dave.c"

# Convert map tiles
echo " - block_tiles.png"
grit block_tiles.png -ftc -gt -gTFF00E1 -gB8 -Mw1 -Mh1 -pu16
strip_timestamp "block_tiles.h"
strip_timestamp "block_tiles.c"

# TODO: other ones
