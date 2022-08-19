#!/bin/bash

# Requires to at least CMAKE v3.0 or HIGHER!
if [ ! -d "build/" ] 
then
    konsole -e /bin/bash --rcfile <(echo "mkdir build/ && cd build/ && cmake ../ && cd .. && cmake --build build/ && build/liv && printf \"\n\nPRESS ENTER TO EXIT...\" && read x && exit")
else
    konsole -e /bin/bash --rcfile <(echo "cmake --build build/ && build/liv && printf \"\n\nPRESS ENTER TO EXIT...\" && read x && exit")
fi    