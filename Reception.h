#ifndef Reception_h
#define Reception_h

#include <queue>
#include "Patient.h"
#include "Doctor.h"
#include <memory>
#include <mutex>

class Reception
{
private:
	int queuePos;
	int numberOfPatientDone;
	std::queue<std::shared_ptr<Patient>> patients;
	std::mutex mtx;
	std::shared_ptr<Doctor> doctorWithSmallestQueue;

	void vacateQueue(std::shared_ptr<Patient> patient, char (*viewArray)[80]);
	void vacateQueue(char (*viewArray)[80]);
	void takeSeat(std::shared_ptr<Patient> patient, char (*viewArray)[80]);
public:
	Reception();
	~Reception();
	int addPatientToQueue(std::shared_ptr<Patient> patient);
	std::shared_ptr<Doctor> receptionTime(char (*viewArray)[80]);
	int getNumberOfPatientDone();
	void setDoctorWithSmallestQueue(std::shared_ptr<Doctor> doctor);
};

#endif

