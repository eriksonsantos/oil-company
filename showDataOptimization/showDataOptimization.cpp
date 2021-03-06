// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma warning(disable:4996)
#include <iostream>
#include <Windows.h>
#include "LinkedList.h"
#include <process.h>
#include <string.h> 

#define	DEL			0x7F

#define Size_Message 38

#define MAX_FileDisk 10

char message[Size_Message];
HANDLE hFile;
bool EXECUTE = TRUE;
DWORD dwBytesRead, dwInit, dwEnd,dwCurrent;
HANDLE hMutexFile,

hSemaphoreDisk;


using namespace std;

struct DataOptimization {
    string  NSEQ,
        TIPO,
        SP_PRESS,
        SP_TEMP,
        VOL,
        INICIO;
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
    DataOptimization aux;
    string output;

    aux.NSEQ = getValue(text, 0, 6);
    aux.SP_PRESS = getValue(text, 10, 16);
    aux.SP_TEMP = getValue(text, 17, 23);
    aux.VOL = getValue(text, 24, 29);
    aux.INICIO = getValue(text, 30, 39);

    output = "NSEQ: " + aux.NSEQ + " SP (TEMP): " + aux.SP_PRESS + "C SP (PRE): " +
        aux.SP_TEMP + "psi VOL: " + aux.VOL + "m3 " + aux.INICIO;



    return(output);
}
unsigned __stdcall  ThreadCloseProgram(LPVOID index);
unsigned __stdcall  ThreadMain(LPVOID index);

HANDLE hQtdDataDisk;
HANDLE hQtdValuesDontRead;
bool bResult = FALSE;
linked_list * lpBuffer;
string *lpBufferAux;
HANDLE hThread[2], hEventESC, hEventWriteDisk, hEventT;
unsigned dwThreadId;
char* qtdDiskData;
char* qtdDiskValuesDontRead;

int main()
{
    hFile = CreateFile(L"../DataOptimization.arq",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

   

    if (hFile == NULL)
        cout << "Error when create/open File. Error type: " << GetLastError() << endl;

    hQtdDataDisk = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"QtdValuesInDisk");

    if (hQtdDataDisk == NULL)
        cout << "Error when  Open FileMapping. Error type: " << GetLastError() << endl;

    qtdDiskData = (char*)MapViewOfFile(
        hQtdDataDisk,
        FILE_MAP_WRITE,
        0,
        0,
        256);

    if (qtdDiskData == NULL)
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

    hMutexFile = OpenMutex(MUTEX_ALL_ACCESS, TRUE, L"mutexFile");

    if (hMutexFile == NULL)
        cout << "Error when open Mutex. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventT = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventT");
    if (hEventT == NULL)  cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventWriteDisk = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventWriteDisk");
    if (hEventWriteDisk == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hSemaphoreDisk = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE,L"SemaphoreDisk");

    if (hSemaphoreDisk == NULL)
        cout << "Error when open Semaphore. Error type: " << GetLastError() << endl;

    hThread[0] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadMain, (LPVOID)0, 0, &dwThreadId);

    if (hThread[0] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThread[1] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)1, 0, &dwThreadId);

    if (hThread[1] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    
    WaitForSingleObject(hThread[0], INFINITE);
    WaitForSingleObject(hThread[1], INFINITE);

    CloseHandle(hMutexFile);
    CloseHandle(hSemaphoreDisk);
    CloseHandle(hFile);

    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
   
    return 0;
}

unsigned __stdcall  ThreadMain(LPVOID index) {
    bool bStatus;
    string aux, lastValue, posDiskFile, sendMessage;
    string qtdValuesDontRead = "0";
    int i = 0;
    char  buff[100];
    DWORD dwPos;
   
    while (EXECUTE) {
        
        WaitForSingleObject(hEventT, INFINITE);
        //WaitForSingleObject(hSemaphoreDisk, INFINITE);
        WaitForSingleObject(hMutexFile, INFINITE);
        
        if(qtdDiskValuesDontRead != NULL)
            qtdValuesDontRead = qtdDiskValuesDontRead;

        while (stoi(qtdValuesDontRead) > 1) {
            if (qtdDiskData != NULL)
                posDiskFile = qtdDiskData;

            if (stoi(posDiskFile) == 1) {
                WaitForSingleObject(hEventWriteDisk, INFINITE);
                dwPos = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
                if (dwPos == INVALID_SET_FILE_POINTER)
                    cout << "Error when use SetFilePointer. Error type: " << GetLastError() << endl;


            }

            bStatus = ReadFile(hFile, &message, Size_Message, &dwBytesRead, NULL);

            if (!bStatus)  cout << "Error when read Disk file. Error type: " << GetLastError() << endl;

            if (stoi(posDiskFile) == 1) {
                lastValue = message;
            }

            aux = message;
            if (aux.length() > 0) {
                sendMessage = generateShowMessage(aux);
                if (stoi(posDiskFile) == 1) {
                    cout << sendMessage << endl;
                    lastValue = sendMessage;

                    qtdValuesDontRead = qtdDiskValuesDontRead;
                    qtdValuesDontRead = to_string(stoi(qtdValuesDontRead) - 1);

                    if (stoi(qtdValuesDontRead) <= 0)
                        qtdValuesDontRead = "0";

                   /* cout << qtdValuesDontRead << endl;*/
                    if(qtdDiskValuesDontRead != NULL)
                        CopyMemory(qtdDiskValuesDontRead,
                            qtdValuesDontRead.c_str(), sizeof(qtdValuesDontRead.c_str()));
                }
                else if(lastValue != sendMessage) {
                    cout << sendMessage << endl;
                    lastValue = sendMessage;

                    qtdValuesDontRead = qtdDiskValuesDontRead;
                    qtdValuesDontRead = to_string(stoi(qtdValuesDontRead) - 1);

                    if (stoi(qtdValuesDontRead) <= 0)
                        qtdValuesDontRead = "0";

                    if(qtdDiskValuesDontRead != NULL)
                        CopyMemory(qtdDiskValuesDontRead,
                            qtdValuesDontRead.c_str(), sizeof(qtdValuesDontRead.c_str()));

                }
            }
        }
         ResetEvent(hEventWriteDisk);
         ReleaseMutex(hMutexFile);
         
            
    }
   
    return 0;
}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;

    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        EXECUTE = FALSE;

       

       
        GetExitCodeThread(hThread[0], &dwExitCode);
        _endthreadex((DWORD)hThread[0]);
        CloseHandle(hThread[0]);
        

        exit;
        InterEnd = FALSE;
    }
   
    return 0;
}

