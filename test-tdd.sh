#!/bin/sh

cmake -G 'Visual Studio 16 2019' -B ./build/
cmake --build build/ --config Debug
./build/bin/Debug/test_client.exe