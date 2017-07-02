#ifndef Patient_h
#define Patient_h

#include <string>

class Patient
{
private:
	std::string name;
	int visitTime;

public:
	Patient();
	Patient(int i);
	~Patient();

	int getVisitTime();
	std::string showName();
};

#endif