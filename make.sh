#!/bin/bash

# unlimit the generation of core file

gcc -g ./src/client.cpp ./util/connection.cpp -o client -I ./include

gcc -g ./util/echo.cpp ./util/connection.cpp ./src/server.cpp -o server -I ./include

gcc -g ./util/connection.cpp ./util/web_server.cpp ./src/tiny.cpp -I ./include/ -o web_server

gcc -g ./util/connection.cpp ./src/adder.cpp -I ./include/ -o cgi_bin

ulimit -c unlimited