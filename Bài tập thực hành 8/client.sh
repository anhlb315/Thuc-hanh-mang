#!/bin/bash

if [ ! -f "server_side/server" ]; then
    echo "[-]Server is not ready"
else
    cd client_side
    make clean
    make
    clear
    echo "[+]Port: $1"
    ./client 127.0.0.1 $1
fi