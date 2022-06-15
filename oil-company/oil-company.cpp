// oil-company.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "DataOptimization.h"
#include <Windows.h>
#include "DataSCADA.h"
#include "DataAlarm.h"
#include <process.h>

unsigned __stdcall  ThreadSCADA(LPVOID index);
unsigned __stdcall  ThreadAlarm(LPVOID index);
unsigned __stdcall ThreadOptimization(LPVOID index);

HANDLE Mutex;
HANDLE hCreateNamedPipe;
bool bConnectNamedPipe;
bool bWriteFile;
string pipeFile[200];
DWORD dwszInputBuffer;
DWORD dwszOutputBuffer;
DWORD dwNoBytesRead;
int main()
{ 
    srand((unsigned)time(0));
    
    HANDLE hThreads[3];
    unsigned dwThreadId;
    string pipeFile[200];
    dwszInputBuffer = sizeof(pipeFile);
    dwszOutputBuffer = sizeof(pipeFile);

    Mutex = CreateMutex(NULL, FALSE, NULL);
    if (Mutex == NULL) 
        cout << "Error when create Mutex. Error type: " << GetLastError();
    
    hCreateNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\MemoryList",
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

    cout << "test";
    if(bConnectNamedPipe == FALSE)
        cout << "Error when create ConectNamedPipe. Error type: " << GetLastError();

    /*DisconnectNamedPipe(hCreateNamedPipe);*/

    /*hCreateNamedPipe = CreateFile(
        L"\\\\.\\pipe\\MemoryList",
        GENERIC_READ |  
        GENERIC_WRITE,
        0,              
        NULL,           
        OPEN_EXISTING,   
        0,         
        NULL);     */   
    

    //if (WaitNamedPipe(L"\\\\.\\pipe\\MemoryList", NMPWAIT_USE_DEFAULT_WAIT) == 0)
    //    printf("\nEsperando por uma instancia do pipe...\n"); // Temporização abortada: o pipe ainda não foi criado
   
    
    hThreads[0] = (HANDLE)
        _beginthreadex(NULL, 0, &ThreadOptimization, (LPVOID)0, 0, &dwThreadId);
    
    if (hThreads[0] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError();
    }

    hThreads[1] = (HANDLE)
       _beginthreadex(NULL, 0, ThreadAlarm, (LPVOID)1, 0, &dwThreadId);
    if (hThreads[1] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError();
    }
    
    hThreads[2] = (HANDLE)
       _beginthreadex(NULL, 0, ThreadSCADA, (LPVOID)2, 0, &dwThreadId);
   
    if (hThreads[2] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError();
    }



   WaitForSingleObject(hThreads[0], INFINITE);
   WaitForSingleObject(hThreads[1], INFINITE);
   WaitForSingleObject(hThreads[2], INFINITE);

   return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    char test[50] = "teste";
    while (1) {
        WaitForSingleObject(Mutex, INFINITE);
        bWriteFile = WriteFile(hCreateNamedPipe, test, dwszOutputBuffer, &dwNoBytesRead, NULL);
        cout << "Optimization: " + data.GenerateData() << endl << endl;

        ReleaseMutex(Mutex);
        Sleep(500); 
    }
    CloseHandle(index);
    CloseHandle(Mutex);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    char test[50] = "teste";
    while (1) {
        WaitForSingleObject(Mutex, INFINITE);
        bWriteFile = WriteFile(hCreateNamedPipe, test, dwszOutputBuffer, &dwNoBytesRead, NULL);
        cout << "Alarme: " + data.GenerateData() << endl << endl;
        ReleaseMutex(Mutex);

        Sleep(1000);
    }
    CloseHandle(index);
    return 0;

}
unsigned __stdcall  ThreadSCADA(LPVOID index) {
    dataSCADA data;
    char test[50] = "teste";
    while (1) {
        WaitForSingleObject(Mutex, INFINITE);
        bWriteFile = WriteFile(hCreateNamedPipe, test, dwszOutputBuffer, &dwNoBytesRead, NULL);
        cout << "SCADA: " + data.GenerateData() << endl << endl;
        ReleaseMutex(Mutex);

        Sleep(1000);
    }
    CloseHandle(index);

    return 0;

}
