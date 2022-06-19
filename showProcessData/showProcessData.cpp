// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <process.h>

bool EXECUTE = TRUE;
HANDLE hCreateNamedPipe;
char message[47];
DWORD dwszInputBuffer = sizeof(message);
DWORD dwszOutputBuffer = sizeof(message);
using namespace std;

unsigned dwThreadId;

HANDLE hEventR,
hEventESC;

HANDLE hThread[2];

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

unsigned __stdcall  ThreadCloseProgram(LPVOID index);
unsigned __stdcall  ThreadMain(LPVOID index);

bool bReadFile, bConnectNamedPipe;

int main()
{
    hEventR = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventR");
    if (hEventR == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;


    hThread[1] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)1, 0, &dwThreadId);

    if (hThread[1] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThread[2] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadMain, (LPVOID)2, 0, &dwThreadId);

    if (hThread[2] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

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


    WaitForSingleObject(hThread[1], INFINITE);
    WaitForSingleObject(hThread[2], INFINITE);

    DisconnectNamedPipe(hCreateNamedPipe);
    CloseHandle(hCreateNamedPipe);

    CloseHandle(hThread[1]);
    CloseHandle(hThread[2]);


    return 0;
}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;

    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        EXECUTE = FALSE;
        cout << "test";

        WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

        for (int i = 0; i < 2; i++) {
            GetExitCodeThread(hThread[i], &dwExitCode);
        }

        exit;
        InterEnd = FALSE;
    }
    //CloseHandle(index);
    return 0;
}

unsigned __stdcall  ThreadMain(LPVOID index) {
    DWORD dwBytesRead = 0;

    while (EXECUTE) {
        WaitForSingleObject(hEventR, INFINITE);

        bReadFile = ReadFile(hCreateNamedPipe, &message, sizeof(message), &dwBytesRead, NULL);
        if (bReadFile == FALSE)
            cout << "ReadFile failed. Error type: " << GetLastError() << endl;
        else
            cout << generateShowMessage(message) << endl;

        Sleep(1000);
    }
    //CloseHandle(index);
    return 0;
}


