// communication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "DataOptimization.h"
#include <Windows.h>
#include "DataProcess.h"
#include "DataAlarm.h"
#include <process.h>
#include "LinkedList.h"

unsigned __stdcall  ThreadProcess(LPVOID index);
unsigned __stdcall  ThreadAlarm(LPVOID index);
unsigned __stdcall ThreadOptimization(LPVOID index);

unsigned __stdcall ThreadRemoveDataProcess(LPVOID index);
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index);
unsigned __stdcall ThreadRemoveDataOptimization(LPVOID index);

HANDLE Mutex;
HANDLE hNamedPipeProcess;
HANDLE hNamedPipeAlarm;
bool bConnectNamedPipe;
bool bWriteFile;


DWORD dwszOutputBufferProcess;
DWORD dwszOutputBufferAlarm;
DWORD dwNoBytesRead;

linked_list gLinked_list;
int main()
{
    srand((unsigned)time(0));

    HANDLE hThreads[6];
    unsigned dwThreadId;

    Mutex = CreateMutex(NULL, FALSE, NULL);
    if (Mutex == NULL)
        cout << "Error when create Mutex. Error type: " << GetLastError() << endl;

    hThreads[0] = (HANDLE)
        _beginthreadex(NULL, 0, &ThreadOptimization, (LPVOID)0, 0, &dwThreadId);

    if (hThreads[0] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[1] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadAlarm, (LPVOID)1, 0, &dwThreadId);
    if (hThreads[1] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[2] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadProcess, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[2] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

   /* hThreads[3] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataProcess, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[3] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }*/

    /*hThreads[4] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataAlarm, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[4] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }*/

    hThreads[5] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataOptimization, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[5] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }


    WaitForSingleObject(hThreads[0], INFINITE);
    WaitForSingleObject(hThreads[1], INFINITE);
    WaitForSingleObject(hThreads[2], INFINITE);
    /*WaitForSingleObject(hThreads[3], INFINITE);
    WaitForSingleObject(hThreads[4], INFINITE);*/
    WaitForSingleObject(hThreads[5], INFINITE);


    return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    string aux;
    while (1) {
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);
        
        ReleaseMutex(Mutex);

        Sleep(500);
    }
    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    string aux;

    while (1) {
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

        Sleep(1000);
    }

    CloseHandle(index);

    return 0;

}
unsigned __stdcall  ThreadProcess(LPVOID index) {
    dataProcess data;
    string aux;

    while (1) {
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

        Sleep(1000);
    }

    CloseHandle(index);

    return 0;

}




unsigned __stdcall  ThreadRemoveDataOptimization(LPVOID index) {
   
    while (1) {
    
        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.showData();
        cout << endl;

        ReleaseMutex(Mutex);
        Sleep(500);
    }
    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index) {
    dataAlarm data;
    string aux;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Alarm", NMPWAIT_USE_DEFAULT_WAIT) == 0)
        cout << "\nEsperando por uma instancia do pipe Alarm...\n" << endl;

    hNamedPipeAlarm = CreateFile(
        L"\\\\.\\pipe\\Alarm",
        GENERIC_READ |
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    while (1) {
        aux = data.GenerateData();
        dwszOutputBufferAlarm = aux.length() + 1;

        bWriteFile = WriteFile(hNamedPipeAlarm, aux.c_str(), dwszOutputBufferAlarm, &dwNoBytesRead, NULL);

        if (!bWriteFile)  cout << "Error when Write PipeAlarm. Error type: " << GetLastError() << endl;

        Sleep(1000);
    }

    CloseHandle(index);
    CloseHandle(hNamedPipeAlarm);

    return 0;

}
unsigned __stdcall  ThreadRemoveDataProcess(LPVOID index) {
    dataProcess data;
    string aux;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Process", NMPWAIT_USE_DEFAULT_WAIT) == 0)
        cout << "\nEsperando por uma instancia do pipe Process...\n" << endl;

    hNamedPipeProcess = CreateFile(
        L"\\\\.\\pipe\\Process",
        GENERIC_READ |
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    while (1) {
        aux = data.GenerateData();
        dwszOutputBufferProcess = aux.length() + 1;

        bWriteFile = WriteFile(hNamedPipeProcess, aux.c_str(), dwszOutputBufferProcess, &dwNoBytesRead, NULL);

        if (!bWriteFile)  cout << "Error when Write PipeProcess. Error type: " << GetLastError() << endl;

        Sleep(1000);
    }

    CloseHandle(index);
    CloseHandle(hNamedPipeProcess);

    return 0;

}
