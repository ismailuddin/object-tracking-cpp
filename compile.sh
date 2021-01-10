#! /bin/bash

if [ -d "build" ]
then
    cd build && rm -rf *
else
    mkdir build && cd $_
fi
cmake ..
cmake --build .
cd ..
./build/TrackObject ./example.mp4 1285 125 125 35
