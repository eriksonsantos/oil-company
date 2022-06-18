// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HANDLE hCreateNamedPipe;
char message[47];
DWORD dwszInputBuffer = sizeof(message);
DWORD dwszOutputBuffer = sizeof(message);
using namespace std;

struct ProcessMessage {
    string  NSEQ,
            PRESSAO_G,
            PRESSAO_T,
            timeStamp,
            NIVEL,
            TEMP;
};
string getValue(string value, int posInit, int posEnd) {
    int i;
    string output = "";
    for (i = posInit; i < posEnd; i++) {
        output = output + value[i];
    }

    return(output);
}
string generateShowMessage(string text) {
    ProcessMessage aux;
    string output;

    aux.NSEQ = getValue(text, 0, 6);
    aux.PRESSAO_G = getValue(text, 24, 30);
    aux.PRESSAO_T = getValue(text, 10, 16);
    aux.TEMP = getValue(text, 17, 23);
    aux.NIVEL = getValue(text, 31, 37);
    aux.timeStamp = getValue(text, 38, 46);

    output = aux.timeStamp + " NSEQ: " + aux.NSEQ + " PR (T): " + aux.PRESSAO_T + "psi TEMP: " +
        aux.TEMP + "C PR (G): " + aux.PRESSAO_G + "psi NIVEL: " + aux.NIVEL + "cm";
        


    return(output);
}

int main()
{
    bool bReadFile, bConnectNamedPipe;

    hCreateNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\Process",
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
            cout << generateShowMessage(message) << endl;

        Sleep(1000);
    }


    DisconnectNamedPipe(hCreateNamedPipe);
    CloseHandle(hCreateNamedPipe);


    return 0;
}

