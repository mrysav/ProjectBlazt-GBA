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
echo " - block_tiles_reduced.png"
grit block_tiles_reduced.png -ftc -mLs -gt -gT'FF00E1' -gB8 -Mw2 -Mh2 -pu16
strip_timestamp "block_tiles_reduced.h"
strip_timestamp "block_tiles_reduced.c"

# TODO: other ones
