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
echo " - block_tiles_1.png"
grit block_tiles_1.png -ftc -mLs -gt -gT'FF00E1' -gB8 -Mw1 -Mh1 -pu16
strip_timestamp "block_tiles_1.h"
strip_timestamp "block_tiles_1.c"

echo " - block_tiles_2.png"
grit block_tiles_2.png -ftc -gt -gT'FF00E1' -gB8 -Mw1 -Mh1 -pu16
strip_timestamp "block_tiles_2.h"
strip_timestamp "block_tiles_2.c"

# TODO: other ones
