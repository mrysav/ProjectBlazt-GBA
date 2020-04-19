#!/bin/bash

GRN="\e[32m"
NC="\e[0m"

MAKE_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
RSC_HOME="$MAKE_HOME/resources/"
# SRC_HOME="$MAKE_HOME/source/"

echo -e "${GRN}Making all resources...${NC}"
cd "$RSC_HOME" || exit
./make_all.sh

cd .. || exit

echo -e "${GRN}Making game...${NC}"
make
