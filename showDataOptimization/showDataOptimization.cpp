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
char message[Size_Message];
HANDLE hFile;
bool EXECUTE = TRUE;
DWORD dwBytesRead, dwInit, dwEnd,dwCurrent;
HANDLE hMutexFile,

hSharedMemoryQtdDisk,

hEventReadFile;


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

HANDLE hFileMap;
bool bResult = FALSE;
linked_list * lpBuffer;
string *lpBufferAux;
HANDLE hThread[2], hEventESC;
unsigned dwThreadId;
char* lpImage;

int main()
{
    hFile = CreateFile(L"../communicationData/DataOptimization.arq",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    hSharedMemoryQtdDisk = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,  
        FALSE,                
        L"QtdValuesInDisk");

    lpImage = (char*)MapViewOfFile(
        hSharedMemoryQtdDisk,
        FILE_MAP_READ| FILE_MAP_WRITE,
        0,					
        0,					
        200);

    if (hFile == NULL)
        cout << "Error when create/open File. Error type: " << GetLastError() << endl;

    hMutexFile = OpenMutex(MUTEX_ALL_ACCESS, TRUE, L"mutexFile");

    if (hMutexFile == NULL)
        cout << "Error when open Mutex. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventReadFile = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventReadFile");
    if (hEventReadFile == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;


    hThread[0] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)0, 0, &dwThreadId);


    hThread[1] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadMain, (LPVOID)1, 0, &dwThreadId);

    if (hThread == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }
    
    WaitForSingleObject(hThread[0], INFINITE);
    WaitForSingleObject(hThread[1], INFINITE);
   
    return 0;
}

unsigned __stdcall  ThreadMain(LPVOID index) {
    bool bStatus;
    string aux, lastValue, qtdValuesInDisk;
    int i = 0;
  
    while (EXECUTE) {
        WaitForSingleObject(hEventReadFile, INFINITE);
        WaitForSingleObject(hMutexFile, INFINITE);

        dwInit = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        dwEnd = SetFilePointer(hFile, 0, NULL, FILE_END);

        dwCurrent = SetFilePointer(hFile, 0, NULL, dwInit);
        i = 0;
        
        qtdValuesInDisk = lpImage;
        while (dwCurrent < dwEnd and stoi(qtdValuesInDisk) > 0) {

            bStatus = ReadFile(hFile, &message, Size_Message, &dwBytesRead, NULL);

            if (!bStatus)  cout << "Error when read Disk file. Error type: " << GetLastError() << endl;
            aux = message;
            if (aux.length() > 0) {
                cout << generateShowMessage(aux) << endl;
            }
            dwCurrent = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

            qtdValuesInDisk = to_string(stoi(qtdValuesInDisk) - 1);
        }
        CopyMemory(lpImage, to_string(0).c_str(), sizeof(to_string(0).c_str()));
        
        dwCurrent = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        
        
        ResetEvent(hEventReadFile);
        ReleaseMutex(hMutexFile);

    
    }

    bStatus = UnmapViewOfFile(lpImage);
    if (!bStatus)  cout << "Error when UnmapViewOfFile. Error type: " << GetLastError() << endl;
    return 0;
}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;

    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        //EXECUTE = FALSE;

        /*WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

        for (int i = 0; i < 2; i++) {
            GetExitCodeThread(hThread[i], &dwExitCode);
        }*/

        exit;
        InterEnd = FALSE;
    }
    //CloseHandle(index);
    return 0;
}

