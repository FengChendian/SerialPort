cmake_minimum_required(VERSION 3.19)
project(SerialPort C)

set(CMAKE_C_STANDARD 11)
include_directories(include)

add_executable(SerialPort main.c include/SerialPort.h src/SerialPort.c)