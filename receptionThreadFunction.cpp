#include "receptionThreadFunction.h"

receptionThreadFunction::receptionThreadFunction(Reception *reception, std::vector<std::shared_ptr<Doctor>> &doctors)
{
    this->reception = reception;
    this->doctors = doctors;
}

void receptionThreadFunction::operator()(std::atomic<bool> &end)
{

    while (reception->getNumberOfPatientDone() != 100)
    {
        int minQueue = 10;
        for (auto doctor : doctors)
        {
            if (doctor->queueSize() < minQueue)
            {
                minQueue = doctor->queueSize();
                reception->setDoctorWithSmallestQueue(doctor);
            }
        }
        if (minQueue == 10)
        {
            reception->setDoctorWithSmallestQueue(nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    bool isEnd = false;
    while (!isEnd)
    {
        isEnd = true;
        for (auto doctor : doctors)
        {
            if (doctor->queueSize() != 0)
            {
                isEnd = false;
                break; 
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::this_thread::sleep_for(std::chrono::seconds(4));
    end = true;
}