#include <time.h>
#include <iomanip>
#include <random>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string formatFloatValue(float value, int tam);

string formatIntValue(int value, int tam);

float randomFloatValue(int floatMin, int floatMax);

int randomIntValue(int IntMin, int IntMax);

int getValue(string message, int posInit, int posEnd);