// oil-company.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <Windows.h>
#include <process.h>

#define ProjetsQTD 6

using namespace std;
int main()
{ 
	bool bCreateProcess[ProjetsQTD] = { NULL };
	STARTUPINFO si[ProjetsQTD];
	PROCESS_INFORMATION pi[ProjetsQTD];
	int i;

	for (i = 0;i < ProjetsQTD; i++) {

		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si[i]);
		ZeroMemory(&pi[i], sizeof(pi[i]));
	}


	bCreateProcess[0] = CreateProcess(
		L"..\\showDataAlarm\\x64\\Debug\\showDataAlarm.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si[0],
		&pi[0]);
		
	if(not bCreateProcess[0])   cout << "Error when create Process Alarm. Error type: " << GetLastError() << endl;
	else {
		cout << "Process Alarm ID: " << pi[0].dwProcessId << endl;
		cout << "Thread ID: " << pi[0].dwThreadId << endl;
	}

	bCreateProcess[1] = CreateProcess(
		L"..\\showProcessData\\x64\\Debug\\showProcessData.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si[1],
		&pi[1]);

	if (not bCreateProcess[1])   cout << "Error when create Process data exhibition. Error type: " << GetLastError() << endl;
	else {
		cout << "Process Data display process ID: " << pi[1].dwProcessId<< endl;
		cout << "Thread ID: " << pi[1].dwThreadId << endl;
	}

	bCreateProcess[2] = CreateProcess(
		L"..\\communicationData\\x64\\Debug\\communicationData.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si[2],
		&pi[2]);

	if (not bCreateProcess[2])   cout << "Error when create Data Process communication. Error type: " << GetLastError() << endl;
	else {
		cout << "Data communication process ID: " << pi[2].dwProcessId << endl;
		cout << "Thread ID: " << pi[2].dwThreadId << endl;
	}

	cout << endl;

	for (i = 0; i < 3; i++) 
		WaitForSingleObject(pi[i].hProcess, INFINITE);

	for (i = 0; i < 3; i++) {
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}
	
	
	
	return 0;
}
