// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "LinkedList.h"
#include <process.h>

#define	DEL			0x7F

#define Size_Message 38
char message[Size_Message];
HANDLE hFile;
bool EXECUTE = TRUE;
DWORD dwBytesRead, dwPos;
HANDLE hMutexFile;


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

int main()
{
    hFile = CreateFile(L"../communicationData/DataOptimization.arq",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == NULL)
        cout << "Error when create/open File. Error type: " << GetLastError() << endl;

    hMutexFile = OpenMutex(MUTEX_ALL_ACCESS, TRUE, L"mutexFile");

    if (hMutexFile == NULL)
        cout << "Error when open Mutex. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
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
    string aux;
    char aux2[1] = "";
    while (EXECUTE) {
        WaitForSingleObject(hMutexFile, INFINITE);
        dwPos = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        bStatus = ReadFile(hFile, &message, Size_Message, &dwBytesRead, NULL);
        
        if (!bStatus)  cout << "Error when read Disk file. Error type: " << GetLastError() << endl;
        aux = message;
        if (aux.length() > 0) {
            cout << generateShowMessage(aux) << endl;

            dwPos = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
            bStatus = WriteFile(hFile, "", Size_Message, &dwBytesRead, NULL);
            if (!bStatus)  cout << "Error when write Disk file. Error type: " << GetLastError() << endl;
        }
        
        
        ReleaseMutex(hMutexFile);

    
    }
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

