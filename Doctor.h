#ifndef Doctor_h
#define Doctor_h

#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include "Patient.h"

class Doctor
{
private:
	std::string name;
	std::queue<std::shared_ptr<Patient>> patients;
	int queuePos;
	std::mutex mtx;
	int timeToEndVisit;
	void vacateQueue(std::shared_ptr<Patient> patient, char (*viewArray)[80]);
	void vacateQueue(char (*viewArray)[80]);
	void takeSeat(std::shared_ptr<Patient> patient, char (*viewArray)[80]);
	int calcRow();

public:
	Doctor();
	Doctor(int i);
	~Doctor();

	void visit(char (*viewArray)[80]);
	int queueSize();
	int addPatientToQueue(std::shared_ptr<Patient> patient);
	int getTimeToEndVisit();
	std::string showName();

};

#endif