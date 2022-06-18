// showProcessData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "LinkedList.h"

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

HANDLE hFileMap;
bool bResult = FALSE;
linked_list * lpBuffer;
string *lpBufferAux;

int main()
{
    cout << "Aqui deverá ser implementado o recebimento dos dados via arquivo em disco" << endl;
    while (1);
    return 0;
}

