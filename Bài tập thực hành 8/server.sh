#!/bin/bash

while :
do
    make clean

    make

    clear

    ./server 8888 &

    sleep 604,800

    pkill server

    echo "[+]Server closed"

    sleep 10
done