#include <time.h>  

class dataOptimization{
public:
	int NSEQ;
	__int8 TIPO[2] = {1,1};
	float SP_PRESS[6];
	float SP_TEMP[6];
	int VOL[5];
	time_t timeStamp;


	dataOptimization();
	void generateData();
};

class dataSCADA {
public:
	int NSEQ[6];
	__int8 TIPO[2] = { 2,2 };
	float PRESSAO_T[6];
	float TEMP[6];
	int PRESSAO_G[6];
	float NIVEL[6];
	time_t timeStamp;

	void generateData();
};

class dataAlarm {
public:
	int NSEQ[6];
	__int8 TIPO[2] = { 5,5 };
	int ID[4];
	int PRIORIDADE[3];
	time_t timeStamp;

	void generateData();
};
