// oil-company.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;
int main()
{ 
	bool bCreateProcess[6] = { NULL };
	STARTUPINFO si[6];
	PROCESS_INFORMATION pi[6];

	ZeroMemory(&si[0], sizeof(si[0]));
	si[0].cb = sizeof(si[0]);
	ZeroMemory(&pi[0], sizeof(pi[0]));

	ZeroMemory(&si[1], sizeof(si[1]));
	si[1].cb = sizeof(si[1]);
	ZeroMemory(&pi[1], sizeof(pi[1]));

	ZeroMemory(&si[2], sizeof(si[2]));
	si[2].cb = sizeof(si[2]);
	ZeroMemory(&pi[2], sizeof(pi[2]));

	ZeroMemory(&si[3], sizeof(si[3]));
	si[3].cb = sizeof(si[3]);
	ZeroMemory(&pi[3], sizeof(pi[3]));


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

	WaitForSingleObject(pi[0].hProcess, INFINITE);
	WaitForSingleObject(pi[1].hProcess, INFINITE);
	WaitForSingleObject(pi[2].hProcess, INFINITE);


	CloseHandle(pi[0].hThread);
	CloseHandle(pi[0].hProcess);

	CloseHandle(pi[1].hThread);
	CloseHandle(pi[1].hProcess);

	CloseHandle(pi[2].hThread);
	CloseHandle(pi[2].hProcess);
	
	return 0;
}
