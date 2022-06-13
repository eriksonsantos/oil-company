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

int main()
{ 
    srand((unsigned)time(0));
    
    HANDLE hThreads[3];
    unsigned dwThreadId;

    hThreads[0] = (HANDLE)
        _beginthreadex(NULL, 0, &ThreadOptimization, (LPVOID)0, 0, &dwThreadId);
   hThreads[1] = (HANDLE)
       _beginthreadex(NULL, 0, ThreadAlarm, (LPVOID)1, 0, &dwThreadId);
   hThreads[2] = (HANDLE)
       _beginthreadex(NULL, 0, ThreadSCADA, (LPVOID)2, 0, &dwThreadId);
   
   WaitForSingleObject(hThreads[0], INFINITE);
   WaitForSingleObject(hThreads[1], INFINITE);
   WaitForSingleObject(hThreads[2], INFINITE);


   return 0;
}
unsigned __stdcall  ThreadOptimization(LPVOID index) {
    dataOptimization data;
    while (1) {
        cout << "Optimization: " + data.GenerateData() << endl;
        Sleep(1000);
    }
    CloseHandle(index);
    return 0;
}
unsigned __stdcall  ThreadAlarm(LPVOID index) {
    dataAlarm data;
    while (1) {
        cout << "Alarme: " + data.GenerateData() << endl;
        Sleep(1000);
    }
    CloseHandle(index);
    return 0;

}
unsigned __stdcall  ThreadSCADA(LPVOID index) {
    dataSCADA data;
    while (1) {
        cout << "SCADA: " + data.GenerateData() << endl;
        Sleep(1000);
    }
    CloseHandle(index);

    return 0;

}
