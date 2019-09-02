#!/bin/bash

for d in `find . -name platformio.ini | sed 's/platformio.ini//' | grep -v .pio`; do
    echo "Building project in folder $d"
    pushd $d
    pio run -s
    popd
done