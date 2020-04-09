#!/bin/bash

function strip_timestamp() {
    FILE="$(grep -vE '^\/\/\sTime' "$1")"
    echo "$FILE" > "$1"
}

# Convert character sprites
echo " - dangerous_dave.png"
grit dangerous_dave.png -m! -ftc -gt -gB8 -Mw2 -Mh2
strip_timestamp "dangerous_dave.h"
strip_timestamp "dangerous_dave.c"

# TODO: other ones
