#!/bin/bash

if [ ! -f "server_side/server" ]; then
    echo "[-]Server is not ready"
else
    cd client_side
    make clean
    make
    clear
    if [ -z "$1" ]
    then
        echo "[+]Port: 8888"
        ./client 127.0.0.1 8888
    else
        echo "[+]Port: $1"
        ./client 127.0.0.1 $1
    fi
fi