#!/bin/bash

RSC_HOME="$(dirname "$0")"
SRC_HOME="$(realpath "$RSC_HOME"/../source/)"

echo -e Making image resources...
cd "$RSC_HOME/images" || exit
./make.sh
mv ./*.h "$SRC_HOME"
mv ./*.c "$SRC_HOME"

echo -e Making map resources...
# TODO
