// ShowOptimizationData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HANDLE hNamedPipe;
DWORD dwszOutputBuffer;
using namespace std;

int main()
{
    hNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\MemoryList",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
        PIPE_UNLIMITED_INSTANCES,
        dwszOutputBuffer,
        dwszOutputBuffer,
        0,
        NULL
    );

         // Todas as instancias estão ocupadas, então espere pelo tempo default 
    if (WaitNamedPipe(L"\\\\.\\pipe\\MemoryList" , NMPWAIT_USE_DEFAULT_WAIT) == 0)
        printf("\nEsperando por uma instancia do pipe..."); // Temporização abortada: o pipe ainda não foi criado

    
    char szReadFileBuffer[100];
    DWORD dwNoBytesRead;
    bool bReadFile;

    while (1) {

        bReadFile = ReadFile(hNamedPipe, szReadFileBuffer, sizeof(szReadFileBuffer), &dwNoBytesRead, NULL);

        if (bReadFile == FALSE)  cout << "Error when read Pipe. Error type: " << GetLastError() << endl;

        /*cout << szReadFileBuffer;*/

    }


}