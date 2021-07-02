#!/bin/bash

RSC_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
SRC_HOME="$(realpath "$RSC_HOME"/../source/)"

echo -e Making map resources...
cd "$RSC_HOME/maps" || exit
./make.sh
mv ./*.h "$SRC_HOME"
mv ./*.c "$SRC_HOME"
