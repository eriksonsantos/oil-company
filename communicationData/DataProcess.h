#include <time.h> 
#include <string>
#include <iostream>
#include "GlobalFunctions.h"


using namespace std;

class dataProcess {
public:
	int NSEQ;
	__int8 TIPO = 22;
	string PRESSAO_T;
	string TEMP;
	string PRESSAO_G;
	string NIVEL;
	string timeStamp;

	dataProcess();
	string GenerateData();
};
