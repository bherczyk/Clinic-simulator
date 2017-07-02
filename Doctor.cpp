#include "Doctor.h"
#include <thread>

using namespace std;

Doctor::Doctor()
{
}

Doctor::Doctor(int i)
{
	name = "D" + to_string(i);
	queuePos = 0;
	timeToEndVisit = 0;
}

Doctor::~Doctor()
{
}

void Doctor::visit(char (*viewArray)[80])
{
	mtx.lock();
	shared_ptr<Patient> patient = patients.front();
	vacateQueue(patient, viewArray);
	takeSeat(patient, viewArray);
	timeToEndVisit = patient->getVisitTime();
	for (timeToEndVisit; timeToEndVisit > 0; timeToEndVisit--)
	{
		this_thread::sleep_for(chrono::seconds(1));
	}
	queuePos--;
	patients.pop();
	vacateQueue(viewArray);
	mtx.unlock();
}

int Doctor::queueSize()
{
	return patients.size();
}

int Doctor::addPatientToQueue(shared_ptr<Patient> patient)
{
	if(patients.size() == 10)
	{
		return -1;
	}
	patients.push(patient);
	queuePos++;
	return queuePos;
}

string Doctor::showName()
{
	return name;
}

int Doctor::getTimeToEndVisit()
{
	return timeToEndVisit;
}

void Doctor::vacateQueue(std::shared_ptr<Patient> patient, char (*viewArray)[80])
{
	int row = calcRow();
	for (int i = 0; i < 4; i++)
	{
		viewArray[row][16 + i] = ' ';
	}
}
void Doctor::vacateQueue(char (*viewArray)[80])
{
	int row = calcRow();
	for (int i = 0; i < 4; i++)
	{
		viewArray[row][11 + i] = ' ';
	}
}
void Doctor::takeSeat(std::shared_ptr<Patient> patient, char (*viewArray)[80])
{
	int row = calcRow();
	for (int i = 0; i < patient->showName().size(); i++)
	{
		viewArray[row][11 + i] = patient->showName()[i];
	}
}

int Doctor::calcRow()
{
	int row;
	if (name == "D0")
		row = 6;
	if (name == "D1")
		row = 11;
	if (name == "D2")
		row = 16;
	if (name == "D3")
		row = 21;
	return row;
}