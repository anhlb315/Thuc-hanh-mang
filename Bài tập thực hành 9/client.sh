#!/bin/bash

if [ ! -f "server_side/server" ]; then
    echo "[-]Server is not ready"
elif [ -f "client_side/client" ]; then
    cd client_side
    if [ -z "$1" ]
    then
        echo "[+]Port: 8888"
        ./client 127.0.0.1 8888
    else
        echo "[+]Port: $1"
        ./client 127.0.0.1 $1
    fi
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