#include <cstdlib>
#include "DataOptimization.h"
#include <Windows.h>


#define maxNseq 999999
using namespace std;


dataOptimization::dataOptimization() {
	this->NSEQ = 0;
	this->SP_PRESS = "";
	this->SP_TEMP = "";
	this->VOL = "";
	this->timeStamp = "";
}

string dataOptimization::GenerateData() {
	if (this->NSEQ < maxNseq) 
		this->NSEQ++;
	else 
		this->NSEQ=1;
	
	float aux = randomFloatValue(0, 10000);
	this->SP_PRESS = formatFloatValue(aux,6);

	aux = randomFloatValue(0, 10000);
	this->SP_TEMP = formatFloatValue(aux, 6);

	
	aux = randomIntValue(0, 10000);
	this->VOL = formatIntValue(aux, 5);

	SYSTEMTIME st;
	GetLocalTime(&st);
	
	this->timeStamp = formatIntValue(st.wHour, 2) + ':' +
					  formatIntValue(st.wMinute,2) + ':' +
					  formatIntValue(st.wSecond,2);
	
	string output = 
			formatIntValue(this->NSEQ, 6) + '|' +
			to_string(this->TIPO) + '|' +
			this->SP_PRESS + '|' +
			this->SP_TEMP + '|' +
			this->VOL + '|' +
			this->timeStamp;

	
	return output;

}