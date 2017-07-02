#include "Reception.h"
#include <thread>
#include <string>

Reception::Reception()
{
	queuePos = 0;
	numberOfPatientDone = 0;
}

Reception::~Reception()
{
}

int Reception::addPatientToQueue(std::shared_ptr<Patient> patient)
{
	patients.push(patient);
	queuePos++;
	return queuePos;
}

std::shared_ptr<Doctor> Reception::receptionTime(char (*viewArray)[80])
{
	mtx.lock();
	std::shared_ptr<Patient> patient = patients.front();
	patients.pop();
	numberOfPatientDone++;
	vacateQueue(patient, viewArray);
	takeSeat(patient, viewArray);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	queuePos--;
	vacateQueue(viewArray);
	mtx.unlock();
	return doctorWithSmallestQueue;
}

int Reception::getNumberOfPatientDone()
{
	return numberOfPatientDone;
}

void Reception::setDoctorWithSmallestQueue(std::shared_ptr<Doctor> doctor)
{
	doctorWithSmallestQueue = doctor;
}

void Reception::vacateQueue(char (*viewArray)[80])
{
	for (int i = 0; i < 4; i++)
	{
		viewArray[1][11 + i] = ' ';
	}
}

void Reception::vacateQueue(std::shared_ptr<Patient> patient, char (*viewArray)[80])
{
	for (int i = 0; i < 4; i++)
	{
		viewArray[1][16 + i] = ' ';
	}
}
void Reception::takeSeat(std::shared_ptr<Patient> patient, char (*viewArray)[80])
{
	for (int i = 0; i < patient->showName().size(); i++)
	{
		viewArray[1][11 + i] = patient->showName()[i];
	}
}