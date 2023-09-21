#!/bin/bash

fDir="$HOME/.local/share/fonts"

mkdir -p "$fDir"
cp ./*.ttf "$fDir/."

fc-cache -f -v | grep "local"
