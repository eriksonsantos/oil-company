#include "GlobalFunctions.h"


string formatFloatValue(float value, int tam) {
	stringstream stream;
	stream << std::fixed << std::setprecision(1) << value;
	string text = stream.str();

	while (text.length() < tam) {
		text = '0' + text;
	}

	return text;
}

string formatIntValue(int value, int tam) {

	string text = to_string(value);

	while (text.length() < tam) {
		text = '0' + text;
	}

	return text;
}

float randomFloatValue(int floatMin, int floatMax) {
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distr(floatMin, floatMax);
	return distr(eng);
}

int randomIntValue(int IntMin, int IntMax) {
	int random = rand() % IntMax;

	return random;
}