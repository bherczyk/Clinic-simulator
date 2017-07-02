#ifndef PatientThreadFunction_h
#define PatientThreadFunction_h

#include "Doctor.h"
#include "Patient.h"
#include "Reception.h"
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <atomic>

class patientThreadFunction
{
  private:
    static const int RowMax = 24;
    static const int ColMax = 80;
    std::shared_ptr<Patient> patient;
    Reception *reception;
    int position;
    char (*viewArray)[ColMax];

    int calcRow(std::string doctorName = "");
    void vacateQueue(std::string doctorName = "");
    void takeSeat(std::string doctorName = "");
    void goToDoctor(std::string doctorName);
    void goToHome(std::string doctorName = "");
    bool checkFreeSeat(std::string doctorName = "");
    
  public:
    patientThreadFunction(int numberOfPatient, Reception *reception, char viewArray[RowMax][ColMax]);
    void operator()();
};

#endif