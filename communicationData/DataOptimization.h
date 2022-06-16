#include "GlobalFunctions.h"

using namespace std;
class dataOptimization {
public:
	int NSEQ;
	__int8 TIPO = 11;
	string SP_PRESS;
	string SP_TEMP;
	string VOL;
	string timeStamp;

	dataOptimization();
	string GenerateData();
};



