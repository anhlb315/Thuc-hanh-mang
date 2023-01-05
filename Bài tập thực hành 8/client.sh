#!/bin/bash

if [ ! -f "server_side/server" ]; then
    echo "[-]Server is not ready"
else
    cd client_side
    make clean
    make
    clear
    ./client 127.0.0.1 8888
fi