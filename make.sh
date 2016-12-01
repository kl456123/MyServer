#!/bin/bash

gcc -g ./src/client.cpp ./util/connection.cpp -o client -I ./include

gcc -g ./util/echo.cpp ./util/connection.cpp ./src/server.cpp -o server -I ./include
