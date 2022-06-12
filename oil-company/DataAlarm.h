#include "GlobalFunctions.h"

using namespace std;
class dataAlarm {
public:
	int NSEQ;
	__int8 TIPO = 55;
	string id;
	string prioridade;
	string timeStamp;

	dataAlarm();
	string GenerateData();
};
