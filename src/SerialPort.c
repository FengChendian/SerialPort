#include "SerialPort.h"
#include "string.h"

HANDLE handler;
bool connected;
COMSTAT status;
DWORD errors;


void openSerialPort(const char *portName)
{
    connected = true;

    handler = CreateFileA(portName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);
    if (handler == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            printf("%s is not available\n", portName);
        } else
        {
            printf("Error!\n");
        }
    } else
    {
        DCB dcbSerialParameters = {0};

        if (!GetCommState(handler, &dcbSerialParameters))
        {
            printf("Failed to get current serial parameters\n");
        } else
        {
            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_DISABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("Could not set serial port parameters\n");
            } else
            {
                connected = true;
                PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(WAIT_TIME);
            }
        }
    }
}

void searchAvailablePort()
{
    char buffer[4];

    char lpTargetPath[5000];

    bool port[20] = {false};

    for (int i = 0; i < 20; ++i)
    {
        char name[10] = "COM";
        itoa(i, buffer, 10);
        strcat(name, buffer);
        DWORD test = QueryDosDevice(name, lpTargetPath, 5000);
        if (test != 0)
        {
            port[i] = true;
            printf("find COM%d in %s\n", i, lpTargetPath);
        }
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            printf("Error!");
        }
    }
    return;
}
// Reading bytes from serial port to buffer;
// returns read bytes count, or if error occurs, returns 0
bool readSerialPort(const char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(handler, &errors, &status);

    if (status.cbInQue > 0)
    {
        if (status.cbInQue > buf_size)
        {
            toRead = buf_size;
        } else
        {
            toRead = status.cbInQue;
        }
    }
    memset((void *) buffer, 0, buf_size);

    if (ReadFile(handler, (LPVOID) buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    }

    return 0;
}

// Sending provided buffer to serial port;
// returns true if succeed, false if not
bool writeSerialPort(const char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(handler, (void *) buffer, buf_size, &bytesSend, 0))
    {
        ClearCommError(handler, &errors, &status);
        return false;
    }

    return true;
}

// Checking if serial port is connected
bool isConnected()
{
    if (!ClearCommError(handler, &errors, &status))
    {
        connected = false;
    }

    return connected;
}

bool close()
{
    return CloseHandle(handler);
}
