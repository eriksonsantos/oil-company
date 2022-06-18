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

