// showAlarmData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>

HANDLE hCreateNamedPipe;
char message[47];
DWORD dwszInputBuffer = sizeof(message);
DWORD dwszOutputBuffer = sizeof(message);

using namespace std;

struct AlarmMessage {
    string  NSEQ,
            MESSAGE,
            ID,
            timeStamp,
            priority
        ;
};

string getMessageAlarm(int ID) {
    if (ID < 1000) return "Message 1";
    if (ID >= 1000 and ID < 2000) return "Message 2";
    if (ID >= 2000 and ID < 3000) return "Message 3";
    if (ID >= 3000 and ID < 4000) return "Message 4";
    if (ID >= 4000 and ID < 5000) return "Message 5";
    if (ID >= 5000 and ID < 6000) return "Message 6";
    if (ID >= 6000 and ID < 7000) return "Message 7";
    if (ID >= 7000 and ID < 8000) return "Message 8";
    if (ID >= 8000 and ID < 9000) return "Message 9";
    if (ID >= 9000 and ID < 10000) return "Message 10";
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


int main()
{
    bool bReadFile, bConnectNamedPipe;

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


    DWORD dwBytesRead = 0;
    bReadFile = TRUE;

    while (TRUE) {

        bReadFile = ReadFile(hCreateNamedPipe, &message, sizeof(message), &dwBytesRead, NULL);
        if (bReadFile == FALSE)
            cout << "ReadFile failed. Error type: " << GetLastError() << endl;
        else
            cout << generateShowMessage(message) << endl;

        Sleep(1000);

    }


    DisconnectNamedPipe(hCreateNamedPipe);
    CloseHandle(hCreateNamedPipe);


    return 0;
}

