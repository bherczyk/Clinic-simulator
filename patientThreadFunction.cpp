#include "patientThreadFunction.h"

patientThreadFunction::patientThreadFunction(int numberOfPatient, Reception *reception, char viewArray[RowMax][ColMax])
{
    this->patient = std::shared_ptr<Patient>(new Patient(numberOfPatient));
    this->reception = reception;
    this->viewArray = viewArray;
}

void patientThreadFunction::vacateQueue(std::string doctorName)
{
    int row = calcRow(doctorName);
    for (int i = 0; i < 4; i++)
    {
        viewArray[row][11 + 5 * position + i] = ' ';
    }
}

void patientThreadFunction::takeSeat(std::string doctorName)
{
    int row = calcRow(doctorName);
    for (int i = 0; i < patient->showName().size(); i++)
    {
        viewArray[row][11 + 5 * position + i] = patient->showName()[i];
    }
}

void patientThreadFunction::goToDoctor(std::string doctorName)
{
    int row = calcRow(doctorName);

    for (int i = 2; i < row; i++)
    {

        for (int j = 0; j < patient->showName().size(); j++)
        {
            viewArray[i][3 + j] = patient->showName()[j];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        for (int j = 0; j < patient->showName().size(); j++)
        {
            viewArray[i][3 + j] = ' ';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}

void patientThreadFunction::goToHome(std::string doctorName)
{
    int row = calcRow(doctorName);

    for (int i = 11; i < ColMax-4; i++)
    {

        for (int j = 0; j < patient->showName().size(); j++)
        {
            viewArray[row+2][j + i] = patient->showName()[j];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        for (int j = 0; j < patient->showName().size(); j++)
        {
            viewArray[row + 2][j + i] = ' ';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

int patientThreadFunction::calcRow(std::string doctorName)
{
    int row;
    if (doctorName == "")
        row = 1;
    if (doctorName == "D0")
        row = 6;
    if (doctorName == "D1")
        row = 11;
    if (doctorName == "D2")
        row = 16;
    if (doctorName == "D3")
        row = 21;
    return row;
}

bool patientThreadFunction::checkFreeSeat(std::string doctorName)
{
    int row = calcRow(doctorName);
    std::string test = "";
    for (int i = 0; i < 4; i++)
    {
        test += viewArray[row][11 + 5 * (position - 1) + i];
    }

    return test == "    ";
}

void patientThreadFunction::operator()()
{
    position = reception->addPatientToQueue(patient);
    takeSeat();
    while (position != 1)
    {
        if (checkFreeSeat())
        {
            vacateQueue();
            position--;
            takeSeat();
        }
    }
    std::shared_ptr<Doctor> doctor = reception->receptionTime(viewArray);
    if (doctor == nullptr)
    {
        goToHome();
        return;
    }
    goToDoctor(doctor->showName());
    position = doctor->addPatientToQueue(patient);
    if(position == -1)
    {
        goToHome(doctor->showName());
        return;
    }
    takeSeat(doctor->showName());
    while (position != 1)
    {
        if (checkFreeSeat(doctor->showName()))
        {
            vacateQueue(doctor->showName());
            position--;
            takeSeat(doctor->showName());
        }
    }
    doctor->visit(viewArray);
    goToHome(doctor->showName());
}