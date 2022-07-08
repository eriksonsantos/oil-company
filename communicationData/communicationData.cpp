// communication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "DataOptimization.h"
#include <Windows.h>
#include "DataProcess.h"
#include "DataAlarm.h"
#include <process.h>
#include "LinkedList.h"
#include <ctime>

unsigned __stdcall  ThreadProcess(LPVOID index);
unsigned __stdcall  ThreadAlarm(LPVOID index);
unsigned __stdcall ThreadOptimization(LPVOID index);

unsigned __stdcall ThreadRemoveDataProcess(LPVOID index);
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index);
unsigned __stdcall ThreadRemoveDataOptimization(LPVOID index);

HANDLE Mutex, hMutexFile;
HANDLE hNamedPipeProcess;
HANDLE hNamedPipeAlarm;

bool bConnectNamedPipe;
bool bWriteFile;

DWORD dwszOutputBufferProcess;
DWORD dwszOutputBufferAlarm;
DWORD dwNoBytesRead,
      dwBytesWritten;

HANDLE hEventC,
hEventO,
hEventP,
hEventA,
hEventESC,

hDiskFile,

hSemaphoreDisk;

HANDLE hThreads[6],
hThreadsCloseProg;
bool EXECUTE = TRUE;

DWORD dwWaitResult , dwPos;

unsigned __stdcall  ThreadCloseProgram(LPVOID index);

linked_list gLinked_list;
int main()
{

    srand((unsigned)time(0));
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

    hMutexFile = OpenMutex(MUTEX_ALL_ACCESS, TRUE, L"mutexFile");

    if (hMutexFile == NULL) 
        cout << "Error when open Mutex. Error type: " << GetLastError() << endl;

    hDiskFile = CreateFile(L"../DataOptimization.arq", 
                           GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,		
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if (hDiskFile == NULL)
        cout << "Error when create File. Error type: " << GetLastError() << endl;

    hSemaphoreDisk = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"SemaphoreDisk");

    if (hSemaphoreDisk == NULL)
        cout << "Error when open Semaphore. Error type: " << GetLastError() << endl;

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
        _beginthreadex(NULL, 0, ThreadRemoveDataProcess, (LPVOID)3, 0, &dwThreadId);

    if (hThreads[3] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[4] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataAlarm, (LPVOID)4, 0, &dwThreadId);

    if (hThreads[4] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[5] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadRemoveDataOptimization, (LPVOID)5, 0, &dwThreadId);

    if (hThreads[5] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreadsCloseProg = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)6, 0, &dwThreadId);

    if (hThreadsCloseProg == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    

    WaitForSingleObject(hThreads[0], INFINITE);
    WaitForSingleObject(hThreads[1], INFINITE);
    WaitForSingleObject(hThreads[2], INFINITE);
    WaitForSingleObject(hThreads[3], INFINITE);
    WaitForSingleObject(hThreads[4], INFINITE);
    WaitForSingleObject(hThreads[5], INFINITE);

    WaitForSingleObject(ThreadCloseProgram, INFINITE);


    for(int i=0; i< 6;i++)
        CloseHandle(hThreads[i]);

    CloseHandle(hEventC);
    CloseHandle(hEventO);
    CloseHandle(hEventP);
    CloseHandle(hEventA);
    CloseHandle(hEventESC);

    CloseHandle(hThreadsCloseProg);


    return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    string aux;
    DWORD dwWaitResultESC;
    HANDLE hTimer;
    BOOL bSucesso;
    LARGE_INTEGER Preset;

    const int nMultiplicadorParaMs = 10000;

    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    
    if (hTimer == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;


   
    while (EXECUTE) {

        srand((unsigned)time(0));
        int period = 1 + (rand() % 5);

        Preset.QuadPart = -(period * 1000 * nMultiplicadorParaMs);

        bSucesso = SetWaitableTimer(hTimer, &Preset, period * 1000, NULL, NULL, FALSE);
        if (bSucesso == NULL)
            cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;

        
        WaitForSingleObject(hEventC, INFINITE);
        
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);
        
        ReleaseMutex(Mutex);
        WaitForSingleObject(hTimer, INFINITE);

    }

    CloseHandle(hTimer);
    //CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    string aux;
    DWORD dwWaitResultESC;
    HANDLE hTimer;
    BOOL bSucesso;
    LARGE_INTEGER Preset;

    //srand((unsigned)time(0));
    //int period =  1 + (rand() % 5);

    const int nMultiplicadorParaMs = 10000;

    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimer == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;
    
    


    while (EXECUTE) {

        srand((unsigned)time(0));
        int period =  1 + (rand() % 5);

        Preset.QuadPart = -(period * 1000 * nMultiplicadorParaMs);


        bSucesso = SetWaitableTimer(hTimer, &Preset, period * 1000, NULL, NULL, FALSE);
        if (bSucesso == NULL)
            cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;

        WaitForSingleObject(hEventC, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);
        WaitForSingleObject(hTimer, INFINITE);

        
    }

    CloseHandle(hTimer);
    //CloseHandle(index);

    return 0;

}
unsigned __stdcall  ThreadProcess(LPVOID index) {
    dataProcess data;
    string aux;
    DWORD dwWaitResultESC;
    HANDLE hTimer;
    BOOL bSucesso;
    LARGE_INTEGER Preset;

    const int nMultiplicadorParaMs = 10000;

    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimer == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;

    Preset.QuadPart = -(500 * nMultiplicadorParaMs);

    bSucesso = SetWaitableTimer(hTimer, &Preset, 500, NULL, NULL, FALSE);
    if (bSucesso == NULL)
        cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;


    while (EXECUTE) {
        WaitForSingleObject(hEventC, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

        WaitForSingleObject(hTimer, INFINITE);
    }

    CloseHandle(hTimer);
    //CloseHandle(index);

    return 0;

}


unsigned __stdcall  ThreadRemoveDataOptimization(LPVOID index) {
    int tam, type, i;
    string message;
    DWORD dwWaitResultESC;
    bool bStatus;

    while (EXECUTE) {
        WaitForSingleObject(hEventO, INFINITE);
        WaitForSingleObject(Mutex, INFINITE);
        

        tam = gLinked_list.getSize();
        i = tam - 1;

        while (i > 0 and gLinked_list.getSize() > 0) {
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 11) {
                gLinked_list.PosRemove(i + 1);

                WaitForSingleObject(hMutexFile, INFINITE);

                bWriteFile = WriteFile(hDiskFile, message.c_str(), message.length(), &dwBytesWritten, NULL);

                if (!bWriteFile)  cout << "Error when Write Disk file. Error type: " << GetLastError() << endl;
                
                ReleaseMutex(hMutexFile);
                ReleaseSemaphore(hSemaphoreDisk, 1, NULL);
                tam--;

            }
            i--;
        }

        ReleaseMutex(Mutex);
    }
       
    

    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index) {
    dataAlarm data;
    int tam,type,i;
    string message, aux;
    DWORD dwWaitResultESC;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Alarm", NMPWAIT_USE_DEFAULT_WAIT) == 0);

    hNamedPipeAlarm = CreateFile(
        L"\\\\.\\pipe\\Alarm",
        GENERIC_READ |
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    while (EXECUTE) {
        WaitForSingleObject(hEventA, INFINITE);
        aux = "";
        WaitForSingleObject(Mutex, INFINITE);
        tam = gLinked_list.getSize();
        i = tam - 1;
        while(i >0 and gLinked_list.getSize() > 0){
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 55) {
                aux = message;
                gLinked_list.PosRemove(i + 1);
                tam--;

                if (aux.length() != 0) {
                    dwszOutputBufferAlarm = aux.length() + 1;

                    bWriteFile = WriteFile(hNamedPipeAlarm, aux.c_str(), dwszOutputBufferAlarm, &dwNoBytesRead, NULL);

                    if (!bWriteFile)  cout << "Error when Write PipeAlarm. Error type: " << GetLastError() << endl;
                    
                    aux = "";
                }
            }
            i--;

        }
       ReleaseMutex(Mutex);

    }
    CloseHandle(hNamedPipeAlarm);
    CloseHandle(index);


    return 0;

}

unsigned __stdcall  ThreadRemoveDataProcess(LPVOID index) {
    dataProcess data;
    string aux, message;
    int tam, type, i;
    DWORD dwWaitResultESC;

    while (WaitNamedPipe(L"\\\\.\\pipe\\Process", NMPWAIT_USE_DEFAULT_WAIT) == 0);

    hNamedPipeProcess = CreateFile(
        L"\\\\.\\pipe\\Process",
        GENERIC_READ |
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    while (EXECUTE) {
        WaitForSingleObject(hEventP, INFINITE);
        aux = "";
        WaitForSingleObject(Mutex, INFINITE);
        
        tam = gLinked_list.getSize();
        i = tam - 1;

        while(i > 0 and gLinked_list.getSize() > 0){
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 22) {
                aux = message;
                gLinked_list.PosRemove(i + 1);
                tam--;
               
                if (aux.length() != 0) {
                    dwszOutputBufferProcess = aux.length() + 1;

                    bWriteFile = WriteFile(hNamedPipeProcess, aux.c_str(), dwszOutputBufferProcess, &dwNoBytesRead, NULL);

                    if (!bWriteFile)  cout << "Error when Write PipeProcess. Error type: " << GetLastError() << endl;
                
                    aux = "";
                }
            }
            i--;

        }
        ReleaseMutex(Mutex);
        
    }

    //CloseHandle(index);
    CloseHandle(hNamedPipeProcess);

    return 0;

}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;
    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        EXECUTE = FALSE;

        WaitForMultipleObjects(5, hThreads, TRUE, INFINITE);

        for (int i = 0; i < 5; i++) {
            GetExitCodeThread(hThreads[i], &dwExitCode);
        }

        exit;
        InterEnd = FALSE;
    }
    //CloseHandle(index);
    return 0;
}
