// oil-company.cpp : This file contains the 'main' function. Program execution begins and ends there.

/* Esse é o programa principal, na qual criará os outros processos.
Nesse sentido, antes de rodar esse programa, deverá gerar os executáveis dos outros programas
escolhendo a opção de 64 Bits (x64) no modo Debug. Com isso feito, o programa será executado 
normalmente.

*/

#pragma warning(disable:4996)

#include <iostream>
#include <Windows.h>
#include <process.h>
#include <string>
#include <conio.h>

HANDLE hEventC,
hEventO,
hEventP,
hEventA,
hEventT,
hEventR,
hEventL,
hEventZ,
hEventESC,

hEventWriteDisk;

HANDLE hThread[2];

char* qtdValuesDisk;
char* qtdValuesDontReadDisk;

HANDLE hMutexFile;
HANDLE hQtdValuesDisk, hQtdValuesDontReadDisk;
HANDLE hSemaphore;
unsigned dwThreadId;

unsigned __stdcall  ThreadkeyboardInput(LPVOID index);
unsigned __stdcall  ThreadCloseProgram(LPVOID index);

bool EXECUTE = TRUE;

#define ProjetsQTD 6
#define ESC 27
#define BUF_SIZE 256
#define MAX_DiskFile 200

using namespace std;
int main()
{ 
	bool bCreateProcess[ProjetsQTD] = { NULL };
	STARTUPINFO si[ProjetsQTD];
	PROCESS_INFORMATION pi[ProjetsQTD];
	string aux;
	int i;
	hMutexFile  = CreateMutex(NULL, FALSE, L"mutexFile");
	if (hMutexFile == NULL)
		cout << "Error when create Mutex. Error type: " << GetLastError() << endl;

	hEventC = CreateEvent(NULL, TRUE, TRUE, L"EventC");
	if (hEventC == NULL) cout << "CreateEvent C failed. Error type: " << GetLastError();

	hEventO = CreateEvent(NULL, TRUE, TRUE, L"EventO");
	if (hEventO == NULL) cout << "CreateEvent O failed. Error type: " << GetLastError();

	hEventP = CreateEvent(NULL, TRUE, TRUE, L"EventP");
	if (hEventP == NULL) cout << "CreateEvent P failed. Error type: " << GetLastError();

	hEventT = CreateEvent(NULL, TRUE, TRUE, L"EventT");
	if (hEventT == NULL) cout << "CreateEvent T failed. Error type: " << GetLastError();

	hEventR = CreateEvent(NULL, TRUE, TRUE, L"EventR");
	if (hEventR == NULL) cout << "CreateEvent R failed. Error type: " << GetLastError();

	hEventL = CreateEvent(NULL, TRUE, TRUE, L"EventL");
	if (hEventL == NULL) cout << "CreateEvent L failed. Error type: " << GetLastError();

	hEventZ = CreateEvent(NULL, FALSE, FALSE, L"EventZ");
	if (hEventZ == NULL) cout << "CreateEvent Z failed. Error type: " << GetLastError();

	hEventA = CreateEvent(NULL, TRUE, TRUE, L"EventA");
	if (hEventA == NULL) cout << "CreateEvent A failed. Error type: " << GetLastError();

	hEventESC = CreateEvent(NULL, TRUE, FALSE, L"EventESC");
	if (hEventESC == NULL) cout << "CreateEvent ESC failed. Error type: " << GetLastError();

	hEventWriteDisk = CreateEvent(NULL, FALSE, FALSE, L"EventWriteDisk");
	if (hEventWriteDisk == NULL) cout << "CreateEvent WriteDisk failed. Error type: " << GetLastError();

	hSemaphore = CreateSemaphore(NULL, 0, MAX_DiskFile, L"SemaphoreDisk");


	if (hSemaphore == NULL)
		cout << "Semaphore failed. Error type: " << GetLastError();

	hQtdValuesDisk = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE,
		0,
		BUF_SIZE,
		L"QtdValuesInDisk");

	if (hQtdValuesDisk == NULL)
		cout << "CreateFileMapping failed. Error type: " << GetLastError();

	qtdValuesDisk = (char*)MapViewOfFile(
		hQtdValuesDisk,
		FILE_MAP_WRITE,
		0,
		0,
		BUF_SIZE);

	if (qtdValuesDisk == NULL) cout << "MapViewOfFile failed. Error type: " << GetLastError();


	hQtdValuesDontReadDisk = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE,
		0,
		BUF_SIZE,
		L"QtdValuesDontRead");

	if (hQtdValuesDontReadDisk == NULL)
		cout << "CreateFileMapping failed. Error type: " << GetLastError();

	qtdValuesDontReadDisk = (char*)MapViewOfFile(
		hQtdValuesDontReadDisk,
		FILE_MAP_WRITE,
		0,
		0,
		BUF_SIZE);

	if (qtdValuesDontReadDisk == NULL) cout << "MapViewOfFile failed. Error type: " << GetLastError();

	
	aux = "0";
	CopyMemory(qtdValuesDisk, aux.c_str(), sizeof(aux.c_str()));
	CopyMemory(qtdValuesDontReadDisk, aux.c_str(), sizeof(aux.c_str()));

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

	bCreateProcess[3] = CreateProcess(
		L"..\\showDataOptimization\\x64\\Debug\\showDataOptimization.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si[2],
		&pi[2]);

	if (not bCreateProcess[3])   cout << "Error when create Process data exhibition. Error type: " << GetLastError() << endl;
	else {
		cout << "Process Data display process ID: " << pi[3].dwProcessId << endl;
		cout << "Thread ID: " << pi[3].dwThreadId << endl;
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
		&si[3],
		&pi[3]);

	if (not bCreateProcess[2])   cout << "Error when create Data Process communication. Error type: " << GetLastError() << endl;
	else {
		cout << "Data communication process ID: " << pi[2].dwProcessId << endl;
		cout << "Thread ID: " << pi[2].dwThreadId << endl;
	}
	
	cout << endl;

	hThread[0] = (HANDLE)
		_beginthreadex(NULL, 0, &ThreadkeyboardInput, (LPVOID)0, 0, &dwThreadId);

	if (hThread == NULL)  cout << "Error when create Thread. Error type: " << GetLastError() << endl;
	
	hThread[1] = (HANDLE)
		_beginthreadex(NULL, 0, &ThreadCloseProgram, (LPVOID)1, 0, &dwThreadId);

	if (hThread == NULL)  cout << "Error when create Thread. Error type: " << GetLastError() << endl;


	WaitForSingleObject(hThread[0], INFINITE);
	WaitForSingleObject(hThread[1], INFINITE);

	for (i = 0; i < 4; i++) 
		WaitForSingleObject(pi[i].hProcess, INFINITE);

	for (i = 0; i < 4; i++) {
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}
	CloseHandle(hEventC);
	CloseHandle(hEventA);
	CloseHandle(hEventESC);
	CloseHandle(hEventL);
	CloseHandle(hEventO);
	CloseHandle(hEventR);
	CloseHandle(hEventT);
	CloseHandle(hEventZ);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	
	return 0;
}

unsigned __stdcall  ThreadkeyboardInput(LPVOID index) {
	char input;
	int flags[9] = { 0 };
	bool bResult;
	while (EXECUTE) {
		input = _getch();
		
		switch (input) {
		case 'c':
			if (flags[0] == 1) {
				bResult = SetEvent(hEventC);
				flags[0] = 0;
				cout << "Tarefa de comunicacao dados desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventC);
				cout << "Tarefa de comunicacao de dados bloqueada" << endl;
				flags[0] = 1;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'o':
			if (flags[1] == 1) {
				bResult = SetEvent(hEventO);
				flags[1] = 0;
				cout << "Tarefa de retirada de dados de otimizacao desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventO);
				flags[1] = 1;
				cout << "Tarefa de retirada de dados de otimizacao bloqueada" << endl;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'p':
			if (flags[2] == 1) {
				bResult = SetEvent(hEventP);
				flags[2] = 0;
				cout << "Tarefa de retirada de dados de processo desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventP);
				cout << "Tarefa de retirada de dados de processo bloqueada" << endl;
				flags[2] = 1;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'a':
			if (flags[3] == 1) {
				bResult = SetEvent(hEventA);
				flags[3] = 0;
				cout << "Tarefa de retirada de alarmes desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventA);
				flags[3] = 1;
				cout << "Tarefa de retirada de alarmes bloqueada" << endl;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 't':
			if (flags[4] == 1) {
				bResult = SetEvent(hEventT);
				flags[4] = 0;
				cout << "Tarefa de exibicao de dados de otimizacao desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventT);
				flags[4] = 1;
				cout << "Tarefa de exibicao de dados de otimizacao bloqueada" << endl;
				

			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'r':
			if (flags[5] == 1) {
				bResult = SetEvent(hEventR);
				flags[5] = 0;
				cout << "Tarefa de exibicao de dados de processo desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventR);
				flags[5] = 1;
				cout << "Tarefa de exibicao de dados de otimizacao bloqueada" << endl;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'l':
			if (flags[6] == 1) {
				bResult = SetEvent(hEventL);
				flags[6] = 0;
				cout << "Tarefa de exibicao de alarmes desbloqueada" << endl;
			}
			else {
				bResult = ResetEvent(hEventL);
				flags[6] = 1;
				cout << "Tarefa de exibicao de alarmes bloqueada" << endl;
			}
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			break;

		case 'z':
			bResult = SetEvent(hEventZ);
			cout << "Console de alarme limpo" << endl;
		
			if (!bResult) {
				cout << "ResetEvent failed. Error type: " << GetLastError();
			}
			break;

		case ESC:
			
			bResult = SetEvent(hEventESC);
			
			if (!bResult) {
				cout << "SetEvent failed. Error type: " << GetLastError();
			}
			
			break;

		default:
			cout << "Caractere invalido." << endl;
			break;
		
		
		}

	}

	_endthreadex((DWORD)index);
	return 0;
}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
	DWORD dwExitCode;
	bool InterEnd = TRUE;
	while (InterEnd) {
		WaitForSingleObject(hEventESC, INFINITE);
		EXECUTE = FALSE;

		for (int i = 0; i < 2; i++) {
			GetExitCodeThread(hThread[i], &dwExitCode);
		}

		exit;
		InterEnd = FALSE;
	}
	_endthreadex((DWORD)index);
	return 0;
}

