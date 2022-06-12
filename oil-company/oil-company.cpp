// oil-company.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma warning(disable:4996)
#include <iostream>
#include "DataOptimization.h"
#include "DataSCADA.h"
#include "DataAlarm.h"


int main()
{
    srand((unsigned)time(0));
    dataOptimization data1;
    dataSCADA data2;
    dataAlarm data3;


    cout << data1.GenerateData() << endl;
    cout << data1.GenerateData() << endl;
    cout << data2.GenerateData() << endl;
    cout << data3.GenerateData() << endl;


    return 0;
}
