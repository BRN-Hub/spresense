#!/usr/bin/env bash

if [ $(basename $(pwd)) != "encoder_host" ]; 
then
  echo "Wrong basedir, execute me in 'encoder_host'"
  exit 1
fi

if ! command -v gum &> /dev/null
then
  echo 'gum not found in $PATH'
  exit 1
fi

cd ../../sdk
# gum confirm "Clean?" && gum spin --spinner monkey --title "Cleaning..." --show-output -- make -j clean
gum spin --spinner monkey --title "Building..." --show-output -- make -j4
# gum confirm "Flash?" && tools/flash.py nuttx.spk
