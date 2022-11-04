#!/bin/bash

# Requires to at least CMAKE v3.0 or HIGHER!
if [ ! -d "builds/" ] 
then
    mkdir builds/ && mkdir builds/linux && cd builds/linux && cmake ../../ && cd ../../ && cmake --build builds/linux && builds/linux/src/liv
else
    cmake --build builds/linux && builds/linux/src/liv
fi
