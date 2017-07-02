#include "Patient.h"
#include <random>

Patient::Patient()
{
}

Patient::Patient(int i)
{
	name = "P" + std::to_string(i);

	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<>range(10, 20);
	visitTime = range(generator);
}


Patient::~Patient()
{
}

int Patient::getVisitTime()
{
	return visitTime;
}

std::string Patient::showName()
{
	return name;
}
