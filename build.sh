#!/bin/bash

if [ ! -d ./build ];then
    echo "mkdir build directory"
    mkdir build
fi

cd build && cmake .. && make && cp examples/simpleClient ../ && cp examples/simpleServer ../ && cp examples/simpleSendfile ../
