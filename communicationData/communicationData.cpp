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

HANDLE hEventC,
hEventO,
hEventP,
hEventA,
hEventESC;

DWORD dwWaitResult;

linked_list gLinked_list;
int main()
{
    srand((unsigned)time(0));

    HANDLE hThreads[6];
    unsigned dwThreadId;

    hEventC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventC");
    if (hEventC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventO = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventO");
    if (hEventO == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventP = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventP");
    if (hEventP == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventA = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventA");
    if (hEventA == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventC");
    if (hEventC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;


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

    hThreads[3] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataProcess, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[3] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[4] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataAlarm, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[4] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[5] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataOptimization, (LPVOID)2, 0, &dwThreadId);

    if (hThreads[5] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    

    WaitForSingleObject(hThreads[0], INFINITE);
    WaitForSingleObject(hThreads[1], INFINITE);
    WaitForSingleObject(hThreads[2], INFINITE);
    WaitForSingleObject(hThreads[3], INFINITE);
    WaitForSingleObject(hThreads[4], INFINITE);
    WaitForSingleObject(hThreads[5], INFINITE);


    for(int i=0; i< 6;i++)
        CloseHandle(hThreads[i]);

    CloseHandle(hEventC);
    CloseHandle(hEventO);
    CloseHandle(hEventP);
    CloseHandle(hEventA);
    CloseHandle(hEventESC);


    return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    string aux;
    DWORD dwWaitResultESC;

    while (1) {
        
        WaitForSingleObject(hEventC, INFINITE);
        
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);
        
        ReleaseMutex(Mutex);

        /*dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
        if (dwWaitResultESC == WAIT_OBJECT_0) exit;*/

        Sleep(1000);
    }
    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    string aux;
    DWORD dwWaitResultESC;

    while (1) {
        WaitForSingleObject(hEventC, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

       /* dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
        if (dwWaitResultESC == WAIT_OBJECT_0) exit;*/

        Sleep(1000);
    }

    CloseHandle(index);

    return 0;

}
unsigned __stdcall  ThreadProcess(LPVOID index) {
    dataProcess data;
    string aux;
    DWORD dwWaitResultESC;

    while (1) {
        WaitForSingleObject(hEventC, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

        dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
        if (dwWaitResultESC != WAIT_OBJECT_0) cout << "test";

        Sleep(1000);
    }

    CloseHandle(index);

    return 0;

}




unsigned __stdcall  ThreadRemoveDataOptimization(LPVOID index) {
    int tam, type;
    string message, aux;
    DWORD dwWaitResultESC;

    while (1) {
        WaitForSingleObject(hEventO, INFINITE);
        aux = "";
        WaitForSingleObject(Mutex, INFINITE);

        tam = gLinked_list.getSize();

        for (int i = 0; i < tam; i++) {
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 11) {
                aux = message;
                gLinked_list.PosRemove(i+1);
                break;
            }

        }
        ReleaseMutex(Mutex);

        /*dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
        if (dwWaitResultESC == WAIT_OBJECT_0) exit;*/
    }
    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index) {
    dataAlarm data;
    int tam,type;
    string message, aux;
    DWORD dwWaitResultESC;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Alarm", NMPWAIT_USE_DEFAULT_WAIT) == 0) {
        cout << "\nEsperando por uma instancia do pipe Alarm...\n" << endl;
        Sleep(100);
    }

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
        WaitForSingleObject(hEventA, INFINITE);
        aux = "";
        WaitForSingleObject(Mutex, INFINITE);
        tam = gLinked_list.getSize();

        for (int i = 0; i < tam; i++) {
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 55) {
                aux = message;
                gLinked_list.PosRemove(i +1);
                break;
            }

        }
       ReleaseMutex(Mutex);

       if (aux.length() != 0) {
           dwszOutputBufferAlarm = aux.length() + 1;

           bWriteFile = WriteFile(hNamedPipeAlarm, aux.c_str(), dwszOutputBufferAlarm, &dwNoBytesRead, NULL);

           if (!bWriteFile)  cout << "Error when Write PipeAlarm. Error type: " << GetLastError() << endl;
       }

      /* dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
       if (dwWaitResultESC == WAIT_OBJECT_0) exit;*/
    }
    CloseHandle(hNamedPipeAlarm);
    CloseHandle(index);


    return 0;

}
unsigned __stdcall  ThreadRemoveDataProcess(LPVOID index) {
    dataProcess data;
    string aux, message;
    int tam, type;
    DWORD dwWaitResultESC;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Process", NMPWAIT_USE_DEFAULT_WAIT) == 0) {
        cout << "\nEsperando por uma instancia do pipe Process...\n" << endl;
        Sleep(100);
    }

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
        WaitForSingleObject(hEventP, INFINITE);
        aux = "";
        WaitForSingleObject(Mutex, INFINITE);
        
        tam = gLinked_list.getSize();

        for (int i = 0; i < tam; i++) {
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 22) {
                aux = message;
                gLinked_list.PosRemove(i + 1);
                break;
            }

        }
        ReleaseMutex(Mutex);
        
        if (aux.length() != 0) {
            dwszOutputBufferProcess = aux.length() + 1;

            bWriteFile = WriteFile(hNamedPipeProcess, aux.c_str(), dwszOutputBufferProcess, &dwNoBytesRead, NULL);

            if (!bWriteFile)  cout << "Error when Write PipeProcess. Error type: " << GetLastError() << endl;
        }

        /*dwWaitResultESC = WaitForSingleObject(hEventESC, INFINITE);
        if (dwWaitResultESC == WAIT_OBJECT_0) exit;*/
    }

    CloseHandle(index);
    CloseHandle(hNamedPipeProcess);

    return 0;

}
