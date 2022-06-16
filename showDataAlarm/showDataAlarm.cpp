// showAlarmData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HANDLE hCreateNamedPipe;
char message[47];
DWORD dwszInputBuffer = sizeof(message);
DWORD dwszOutputBuffer = sizeof(message);

using namespace std;

int main()
{
    bool bReadFile, bConnectNamedPipe;

    hCreateNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\Alarm",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
        PIPE_UNLIMITED_INSTANCES,
        dwszOutputBuffer,
        dwszInputBuffer,
        0,
        NULL
    );
    if (hCreateNamedPipe == INVALID_HANDLE_VALUE)
        cout << "Error when create NamedPipe. Error type: " << GetLastError();

    bConnectNamedPipe = ConnectNamedPipe(hCreateNamedPipe, NULL);

    if (bConnectNamedPipe == FALSE)
        cout << "Error when create ConectNamedPipe. Error type: " << GetLastError();


    DWORD dwBytesRead = 0;
    bReadFile = TRUE;

    while (TRUE) {

        bReadFile = ReadFile(hCreateNamedPipe, &message, sizeof(message), &dwBytesRead, NULL);
        if (bReadFile == FALSE)
            cout << "ReadFile failed. Error type: " << GetLastError() << endl;
        else
            cout << message << endl;

        Sleep(1000);

    }


    DisconnectNamedPipe(hCreateNamedPipe);
    CloseHandle(hCreateNamedPipe);


    return 0;
}

