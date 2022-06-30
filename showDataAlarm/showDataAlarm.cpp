// showAlarmData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <process.h>

HANDLE hCreateNamedPipe;
char message[47];
DWORD dwszInputBuffer = sizeof(message);
DWORD dwszOutputBuffer = sizeof(message);

using namespace std;

HANDLE hEventZ,
hEventL,
hEventESC,
hThread[2];

DWORD dwBytesRead = 0,
dwWaitResultESC;

struct AlarmMessage {
    string  NSEQ,
            MESSAGE,
            ID,
            timeStamp,
            priority
        ;
};

bool EXECUTE = TRUE;

string getMessageAlarm(int ID) {
    if (ID < 1000) return "Alta pressao no poco##########";
    if (ID >= 1000 and ID < 2000) return "Alta temperatura no poco######";
    if (ID >= 2000 and ID < 3000) return "Alto volume no poco###########";
    if (ID >= 3000 and ID < 4000) return "Alta pressao no tubo##########";
    if (ID >= 4000 and ID < 5000) return "Alta temperatura no tubo######";
    if (ID >= 5000 and ID < 6000) return "Baixa efic de bombeamento#####";
    if (ID >= 6000 and ID < 7000) return "Temp alta no tanque de armaz##";
    if (ID >= 7000 and ID < 8000) return "Press alta no tanque de armaz#";
    if (ID >= 8000 and ID < 9000) return "Stress alto nas hastes########";
    if (ID >= 9000 and ID < 10000) return "Falha de comunicacao##########";
}

string getValue(string value, int posInit, int posEnd) {
    int i;
    string output = "";
    for (i = posInit; i < posEnd; i++) {
        output = output + value[i];
    }

    return(output);
}
string generateShowMessage(string text) {
    AlarmMessage aux;
    string output;

    aux.NSEQ = getValue(text, 0, 6);
    aux.ID = getValue(text, 10, 14);
    aux.priority = getValue(text, 15, 18);
    aux.timeStamp = getValue(text, 19, 27);

    output = aux.timeStamp + " NSEQ:" + aux.NSEQ + " " + 
             getMessageAlarm(stoi(aux.ID)) +  " PRI:" + aux.priority;


    return(output);
}

unsigned __stdcall  ThreadClearConsole(LPVOID index);
unsigned __stdcall  ThreadMain(LPVOID index);
unsigned __stdcall  ThreadCloseProgram(LPVOID index);

unsigned dwThreadId;
bool bReadFile, bConnectNamedPipe;
int main()
{
    hEventZ = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventZ");
    if (hEventZ == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventL = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventL");
    if (hEventL == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hEventESC = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"EventESC");
    if (hEventESC == NULL)
        cout << "Error when OpenEvent. Error type: " << GetLastError() << endl;

    hThread[1] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadClearConsole, (LPVOID)1, 0, &dwThreadId);

    if (hThread[1] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThread[2] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadMain, (LPVOID)2, 0, &dwThreadId);

    if (hThread[2] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }

    hThread[3] = (HANDLE)
        _beginthreadex(NULL, 0, ThreadCloseProgram, (LPVOID)3, 0, &dwThreadId);

    if (hThread[3] == NULL) {
        cout << "Error when create Thread. Error type: " << GetLastError() << endl;
    }


    hCreateNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\Alarm",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
        PIPE_UNLIMITED_INSTANCES,
        dwszOutputBuffer,
        dwszInputBuffer,
        0,
        NULL
    );
    if (hCreateNamedPipe == INVALID_HANDLE_VALUE)
        cout << "Error when create NamedPipe. Error type: " << GetLastError();

    bConnectNamedPipe = ConnectNamedPipe(hCreateNamedPipe, NULL);

    if (bConnectNamedPipe == FALSE)
        cout << "Error when create ConectNamedPipe. Error type: " << GetLastError();

    WaitForSingleObject(hThread[1], INFINITE);
    WaitForSingleObject(hThread[2], INFINITE);
    WaitForSingleObject(hThread[3], INFINITE);

    DisconnectNamedPipe(hCreateNamedPipe);
    CloseHandle(hCreateNamedPipe);

    CloseHandle(hThread[1]);
    CloseHandle(hThread[2]);
    CloseHandle(hThread[3]);


    return 0;
}

unsigned __stdcall  ThreadClearConsole(LPVOID index) {
    DWORD dwWaitResult;
    while (EXECUTE) {
        dwWaitResult = WaitForSingleObject(hEventZ, 500);
        if (dwWaitResult == WAIT_OBJECT_0)
            system("CLS");
    }
    
    //CloseHandle(index);
    return 0;
}

unsigned __stdcall  ThreadMain(LPVOID index) {
    bReadFile = TRUE;
    while (EXECUTE) {
        WaitForSingleObject(hEventL, INFINITE);

        bReadFile = ReadFile(hCreateNamedPipe, &message, sizeof(message), &dwBytesRead, NULL);
        if (bReadFile == FALSE)
            cout << "ReadFile failed. Error type: " << GetLastError() << endl;
        else
            cout << generateShowMessage(message) << endl;

    }
    //CloseHandle(index);
    return 0;
}

unsigned __stdcall  ThreadCloseProgram(LPVOID index) {
    DWORD dwExitCode;
    bool InterEnd = TRUE;
    bReadFile = TRUE;
    while (InterEnd) {
        WaitForSingleObject(hEventESC, INFINITE);
        EXECUTE = FALSE;

        WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

        for (int i = 0; i < 2; i++) {
            GetExitCodeThread(hThread[i], &dwExitCode);
        }

        exit;
        InterEnd = FALSE;
    }
    //CloseHandle(index);
    return 0;
}

