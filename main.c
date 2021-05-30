#include <stdio.h>
#include "SerialPort.h"

const char *portName = "COM3";

extern bool connected;

#define DATA_LENGTH 16

char data[MAX_DATA_LENGTH];

int main()
{
    searchAvailablePort();

    openSerialPort(portName);

    Sleep(1000);
    if (isConnected())
    {
        int readSuccess = readSerialPort(data, DATA_LENGTH);
        if (readSuccess)
        {
            printf("%s", data);
        }
    }
    printf("%d",close());
    return 0;
}
