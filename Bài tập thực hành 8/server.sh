#!/bin/bash

while :
do
    cd server_side
    
    make clean

    make

    clear

    ./server 8888 &

    cd ..

    sleep 604,800

    pkill server

    echo "[+]Server closed"

    sleep 10
done