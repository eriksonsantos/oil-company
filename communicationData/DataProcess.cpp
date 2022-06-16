#include <cstdlib>
#include "DataProcess.h"
#include <iostream>
#include <Windows.h>


#define maxNseq 999999
using namespace std;


dataProcess::dataProcess() {
	this->NSEQ = 0;
	this->NIVEL = "";
	this->PRESSAO_G = "";
	this->PRESSAO_T = "";
	this->timeStamp = "";
	this->TEMP = "";
}

string dataProcess::GenerateData() {
	if (this->NSEQ < maxNseq)
		this->NSEQ++;
	else
		this->NSEQ = 1;

	float aux = randomFloatValue(0, 10000);
	this->NIVEL = formatFloatValue(aux, 6);

	aux = randomFloatValue(0, 10000);
	this->PRESSAO_G = formatFloatValue(aux, 6);


	aux = randomIntValue(0, 10000);
	this->PRESSAO_T = formatFloatValue(aux, 6);

	aux = randomIntValue(0, 10000);
	this->TEMP = formatFloatValue(aux, 6);

	SYSTEMTIME st;
	GetLocalTime(&st);

	this->timeStamp = formatIntValue(st.wHour, 2) + ':' +
		formatIntValue(st.wMinute, 2) + ':' +
		formatIntValue(st.wSecond, 2);

	string output =
		formatIntValue(this->NSEQ,6) + '|' +
		to_string(this->TIPO) + '|' +
		this->PRESSAO_T + '|' +
		this->TEMP + '|' +
		this->PRESSAO_G + '|' +
		this->NIVEL + '|' +
		this->timeStamp;


	return output;

}