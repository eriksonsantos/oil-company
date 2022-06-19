// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "LinkedList.h"
#include <process.h>

#define Size_Message 47
char message[Size_Message];

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
    aux.SP_TEMP = getValue(text, 18, 24);
    aux.VOL = getValue(text, 26, 31);
    aux.INICIO = getValue(text, 32, 40);

    output = "NSEQ: " + aux.NSEQ + " SP (TEMP): " + aux.SP_PRESS + "C SP (PRE): " +
        aux.SP_TEMP + "psi VOL: " + aux.VOL + "m3 PROD: " + aux.INICIO;



    return(output);
}
unsigned __stdcall  ThreadCloseProgram(LPVOID index);

HANDLE hFileMap;
bool bResult = FALSE;
linked_list * lpBuffer;
string *lpBufferAux;
HANDLE hThread, hEventESC;
unsigned dwThreadId;

int main()
{
    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    cout << "Os dados serão mostrados na segunda etapa." << endl;
    hThread = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)0, 0, &dwThreadId);

    if (hThread == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }
    
    WaitForSingleObject(hThread, INFINITE);
   
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

