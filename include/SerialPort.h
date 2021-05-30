//
// Created by FengChendian on 2021/5/30.
//

#ifndef SERIALPORT_SERIALPORT_H
#define SERIALPORT_SERIALPORT_H

#define MAX_DATA_LENGTH 255
#define WAIT_TIME 100

#include <windows.h>
#include <stdio.h>
#include "stdbool.h"

void searchAvailablePort();
void openSerialPort(const char *portName);
bool readSerialPort(const char *buffer, unsigned int buf_size);
bool writeSerialPort(const char *buffer, unsigned int buf_size);
bool isConnected();
bool close();

#endif //SERIALPORT_SERIALPORT_H
