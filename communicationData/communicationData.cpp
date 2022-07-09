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

unsigned __stdcall ThreadVerifyListSize(LPVOID index);

unsigned __stdcall ThreadRemoveDataProcess(LPVOID index);
unsigned __stdcall  ThreadRemoveDataAlarm(LPVOID index);
unsigned __stdcall ThreadRemoveDataOptimization(LPVOID index);

unsigned __stdcall ThreadVerifyQtdDataDontReadInDiskFile(LPVOID index);

HANDLE Mutex, hMutexFile, hMutexValuesDontRead;
HANDLE hNamedPipeProcess;
HANDLE hNamedPipeAlarm;

bool bConnectNamedPipe;
bool bWriteFile;

HANDLE hTimerOptmization,
hTimerAlarm,
hTimerProcess;

const int nConvertToMs = 10000;
BOOL bSucess;
LARGE_INTEGER Preset;

DWORD dwszOutputBufferProcess,
    dwszOutputBufferAlarm,
    dwNoBytesRead,
    dwBytesWritten,
    dwBytesRead;

HANDLE hEventC,
hEventO,
hEventP,
hEventA,
hEventESC,
hEventWriteDisk,

hDiskFile,
hEventFullList,

hEventFullFileDisk,
hSemaphoreDisk,


hQtdDataDisk,
hQtdValuesDontRead;

HANDLE hThreads[8],
hThreadsCloseProg;

bool EXECUTE = TRUE;
char* qtdValuesDisk;
char* qtdDiskValuesDontRead;

string qtdValuesDontRead,posDiskFile;

DWORD dwWaitResult , dwPos;

unsigned __stdcall  ThreadCloseProgram(LPVOID index);

linked_list gLinked_list;

#define MAX_FileDisk 200
#define Max_SizeList 100

using namespace std;
int main()
{
    qtdValuesDontRead = "0";

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

    hEventFullList = CreateEvent(NULL, FALSE, FALSE, L"EventFullList");
    if (hEventFullList == NULL) 
        cout << "CreateEvent FullList failed. Error type: " << GetLastError();

    hEventFullFileDisk = CreateEvent(NULL, FALSE, FALSE, L"EventFullFileDisk");
    if (hEventFullFileDisk == NULL)
        cout << "CreateEvent FullFileDisk failed. Error type: " << GetLastError();

    hEventC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventC");
    if (hEventC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventWriteDisk = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventWriteDisk");
    if (hEventWriteDisk == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    Mutex = CreateMutex(NULL, FALSE, NULL);
    if (Mutex == NULL)
        cout << "Error when create Mutex. Error type: " << GetLastError() << endl;

    Mutex = CreateMutex(NULL, FALSE, NULL);
    if (Mutex == NULL)
        cout << "Error when create Mutex. Error type: " << GetLastError() << endl;


    hMutexValuesDontRead = CreateMutex(NULL, FALSE, NULL);

    if (hMutexValuesDontRead == NULL)
        cout << "Error when create Mutex. Error type: " << GetLastError() << endl;

    hDiskFile = CreateFile(L"../DataOptimization.arq", 
                           GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,		
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if (hDiskFile == NULL)
        cout << "Error when create File. Error type: " << GetLastError() << endl;


    hQtdDataDisk = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"QtdValuesInDisk");

    if (hQtdDataDisk == NULL)
        cout << "Error when  Open FileMapping. Error type: " << GetLastError() << endl;

    qtdValuesDisk = (char*)MapViewOfFile(
        hQtdDataDisk,
        FILE_MAP_WRITE,
        0,
        0,
        256);

    if (qtdValuesDisk == NULL)
        cout << "Error when MapViewOfFile. Error type: " << GetLastError() << endl;

    hQtdValuesDontRead = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"QtdValuesDontRead");

    if (hQtdValuesDontRead == NULL)
        cout << "Error when  Open FileMapping. Error type: " << GetLastError() << endl;

    qtdDiskValuesDontRead = (char*)MapViewOfFile(
        hQtdValuesDontRead,
        FILE_MAP_WRITE,
        0,
        0,
        256);

    if (qtdDiskValuesDontRead == NULL)
        cout << "Error when MapViewOfFile. Error type: " << GetLastError() << endl;

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

    hThreads[6] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadVerifyListSize, (LPVOID)6, 0, &dwThreadId);

    if (hThreads[6] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThreads[7] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadVerifyQtdDataDontReadInDiskFile, (LPVOID)7, 0, &dwThreadId);

    if (hThreads[7] == NULL) {
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

    WaitForSingleObject(hThreads[6], INFINITE);
    WaitForSingleObject(hThreads[7], INFINITE);

    WaitForSingleObject(ThreadCloseProgram, INFINITE);

    CloseHandle(hTimerOptmization);
    CloseHandle(hTimerAlarm);
    CloseHandle(hTimerProcess);


    for(int i=0; i< 7;i++)
        CloseHandle(hThreads[i]);

    CloseHandle(hEventC);
    CloseHandle(hEventO);
    CloseHandle(hEventP);
    CloseHandle(hEventA);
    CloseHandle(hEventESC);
    CloseHandle(hEventFullList);

    CloseHandle(hThreadsCloseProg);


    return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    string aux;
    DWORD dwWaitResultESC;
    //HANDLE hTimer;
    //BOOL bSucesso;
    //LARGE_INTEGER Preset;

    //const int nMultiplicadorParaMs = 10000;

    hTimerOptmization = CreateWaitableTimer(NULL, FALSE, NULL);
    
    if (hTimerOptmization == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;


   
    while (EXECUTE) {

        srand((unsigned)time(0));
        int period = 1 + (rand() % 5);

        Preset.QuadPart = -(period * 1000 * nConvertToMs);

        bSucess = SetWaitableTimer(hTimerOptmization, &Preset, period * 1000, NULL, NULL, FALSE);
        if (bSucess == NULL)
            cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;

        WaitForSingleObject(hEventFullList, INFINITE);
        WaitForSingleObject(hEventC, INFINITE);

        
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);
        
        ReleaseMutex(Mutex);
        WaitForSingleObject(hTimerOptmization, INFINITE);

    }

    //CloseHandle(hTimer);
    //CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    string aux;
    DWORD dwWaitResultESC;
    //HANDLE hTimer;
    //BOOL bSucesso;
    //LARGE_INTEGER Preset;

    //const int nMultiplicadorParaMs = 10000;

    hTimerAlarm = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimerAlarm == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;
    
    


    while (EXECUTE) {

        srand((unsigned)time(0));
        int period =  1 + (rand() % 5);

        Preset.QuadPart = -(period * 1000 * nConvertToMs);


        bSucess = SetWaitableTimer(hTimerAlarm, &Preset, period * 1000, NULL, NULL, FALSE);
        if (bSucess == NULL)
            cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;

        WaitForSingleObject(hEventC, INFINITE);
        WaitForSingleObject(hEventFullList, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);
        WaitForSingleObject(hTimerAlarm, INFINITE);

        
    }

    //CloseHandle(hTimer);
    _endthreadex((DWORD)index);

    return 0;

}
unsigned __stdcall  ThreadProcess(LPVOID index) {
    dataProcess data;
    string aux;
    DWORD dwWaitResultESC;
    //HANDLE hTimer;
    //BOOL bSucesso;
    //LARGE_INTEGER Preset;

    //const int nMultiplicadorParaMs = 10000;

    hTimerProcess = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimerProcess == NULL)
        cout << "Error when create Timer. Error type: " << GetLastError() << endl;

    Preset.QuadPart = -(500 * nConvertToMs);

    bSucess = SetWaitableTimer(hTimerProcess, &Preset, 500, NULL, NULL, FALSE);
    if (bSucess == NULL)
        cout << "Error in SetWaitableTimer. Error type: " << GetLastError() << endl;


    while (EXECUTE) {
        WaitForSingleObject(hEventFullList, INFINITE);
        WaitForSingleObject(hEventC, INFINITE);
        aux = data.GenerateData();

        WaitForSingleObject(Mutex, INFINITE);
        gLinked_list.PosInsert(aux, 1);

        ReleaseMutex(Mutex);

        WaitForSingleObject(hTimerProcess, INFINITE);
    }

    //CloseHandle(hTimer);
    _endthreadex((DWORD)index);

    return 0;

}


unsigned __stdcall  ThreadRemoveDataOptimization(LPVOID index) {
    int tam, type, i;
    string message;
    DWORD dwWaitResultESC, dwPos;
    bool bStatus;

    posDiskFile = "0";
    while (EXECUTE) {
        WaitForSingleObject(hEventO, INFINITE);
        WaitForSingleObject(hEventFullFileDisk, INFINITE);
        WaitForSingleObject(Mutex, INFINITE);

        tam = gLinked_list.getSize();
        i = tam - 1;

        while (i > 0 and gLinked_list.getSize() > 0) {
            message = gLinked_list.getValue(i);
            type = getValue(message, 7, 9);

            if (type == 11) {
                gLinked_list.PosRemove(i + 1);

                WaitForSingleObject(hMutexFile, INFINITE);
                posDiskFile = qtdValuesDisk;
                if (stoi(posDiskFile) >= MAX_FileDisk) {
                    dwPos = SetFilePointer(hDiskFile, 0, NULL, FILE_BEGIN);
                    if (dwPos == INVALID_SET_FILE_POINTER)
                        cout << "Error when use SetFilePointer. Error type: " << GetLastError() << endl;
                  

                    posDiskFile = "0";

                }
                WaitForSingleObject(hMutexValuesDontRead, INFINITE);
                qtdValuesDontRead = qtdDiskValuesDontRead;
                ReleaseMutex(hMutexValuesDontRead);

                if (stoi(qtdValuesDontRead) == 0) {
                    dwPos = SetFilePointer(hDiskFile, 0, NULL, FILE_BEGIN);
                    if (dwPos == INVALID_SET_FILE_POINTER)
                        cout << "Error when use SetFilePointer. Error type: " << GetLastError() << endl;


                    posDiskFile = "0";

                }

                posDiskFile = to_string(stoi(posDiskFile) + 1);
           
                if(qtdValuesDisk != NULL and posDiskFile.length() > 0)
                    CopyMemory(qtdValuesDisk, posDiskFile.c_str(),
                        sizeof(posDiskFile.c_str()));

                bWriteFile = WriteFile(hDiskFile, message.c_str(), message.length(), &dwBytesWritten, NULL);

                if (!bWriteFile)  cout << "Error when Write Disk file. Error type: " << GetLastError() << endl;
                
                
              
                qtdValuesDontRead = to_string(stoi(qtdValuesDontRead) + 1);

                if (stoi(qtdValuesDontRead) >= MAX_FileDisk)
                    qtdValuesDontRead = to_string(MAX_FileDisk);

                if (qtdValuesDisk != NULL and qtdValuesDontRead.length() > 0)
                    CopyMemory(qtdDiskValuesDontRead, qtdValuesDontRead.c_str(),
                        sizeof(qtdValuesDontRead.c_str()));

               
                SetEvent(hEventWriteDisk);
                ReleaseMutex(hMutexFile);
                ReleaseSemaphore(hSemaphoreDisk, 1, NULL);
                tam--;

            }
            i--;
        }

        ReleaseMutex(Mutex);
    }
       
    

    _endthreadex((DWORD)index);
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
    _endthreadex((DWORD)index);


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

    
    CloseHandle(hNamedPipeProcess);
    _endthreadex((DWORD)index);
    return 0;

}

unsigned __stdcall ThreadVerifyQtdDataDontReadInDiskFile(LPVOID index) {
    int tam, first,i;
    first = 0;
    tam = 0;
    string aux;
    while (EXECUTE) {
        //WaitForSingleObject(hMutexValuesDontRead, INFINITE);
        /*if(stoi(qtdValuesDontRead) != MAX_FileDisk)*/  

        if (qtdDiskValuesDontRead != NULL) {
            aux = qtdDiskValuesDontRead;
            tam = stoi(aux);

        }

        /*cout << "\nQTD Values dont read: " << qtdValuesDontRead << endl;
        cout << "\nPosicao : " << posDiskFile << endl;*/
       
        /*if (qtdDiskValuesDontRead != NULL and qtdValuesDontRead.length() > 0) {
            tam = stoi(qtdValuesDontRead);

           CopyMemory(qtdDiskValuesDontRead,
                qtdValuesDontRead.c_str(), sizeof(qtdValuesDontRead.c_str()));

        }
        ReleaseMutex(hMutexValuesDontRead);*/

        if (tam >= MAX_FileDisk) {
            if (first == 0) {
                cout << "A tarefa de retirada de dados de otimizacao foi bloqueada.\n";
                first = 1;

            }
           
            ResetEvent(hEventFullFileDisk);
        }
        else {
            SetEvent(hEventFullFileDisk);

            if (first == 1) {
                cout << "A tarefa de retirada de dados de otimizacao foi desbloqueada.\n";
                first = 0;
            }

        }


    }
    return 0;
}

unsigned __stdcall ThreadVerifyListSize(LPVOID index) {
    int tam, first;
    first = 0;
    while (EXECUTE) {
        WaitForSingleObject(Mutex, INFINITE);
        tam = gLinked_list.getSize();
        
        if (tam >= Max_SizeList) {
            if (first == 0) {
                cout << "A tarefa de comunicacao de dados foi bloqueada.\n";
                first = 1;
            }

            ResetEvent(hEventFullList);
        }
        else {
            SetEvent(hEventFullList);

            if (first == 1) {
                cout << "A tarefa de comunicacao de dados foi desbloqueada.\n";
                first = 0;
            }
            
        }
        ReleaseMutex(Mutex);

        
    }
    return 0;

}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;
    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        EXECUTE = FALSE;

        //WaitForMultipleObjects(5, hThreads, TRUE, INFINITE);

        for (int i = 0; i < 5; i++) {
            GetExitCodeThread(hThreads[i], &dwExitCode);
        }

        exit;
        InterEnd = FALSE;
    }
    _endthreadex((DWORD)index);
    return 0;
}
