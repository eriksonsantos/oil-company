#include <cstdlib>
#include "DataAlarm.h"
#include <iostream>
#include <Windows.h>


#define maxNseq 999999
using namespace std;


dataAlarm::dataAlarm() {
	this->NSEQ = 0;
	this->id = "";
	this->prioridade= "";
	this->timeStamp = "";

}

string dataAlarm::GenerateData() {
	if (this->NSEQ < maxNseq)
		this->NSEQ++;
	else
		this->NSEQ = 1;

	float aux = randomIntValue(0, 10000);
	this->id = formatIntValue(aux, 4);

	aux = randomIntValue(0, 1000);
	this->prioridade = formatIntValue(aux, 3);


	SYSTEMTIME st;
	GetLocalTime(&st);

	this->timeStamp = formatIntValue(st.wHour, 2) + ':' +
		formatIntValue(st.wMinute, 2) + ':' +
		formatIntValue(st.wSecond, 2);

	string output =
		formatIntValue(this->NSEQ, 6) + '|' +
		to_string(this->TIPO) + '|' +
		this->id + '|' +
		this->prioridade + '|' +
		this->timeStamp;

	return output;

}