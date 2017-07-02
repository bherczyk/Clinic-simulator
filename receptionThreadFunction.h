#ifndef ReceptionThreadFunction_h
#define ReceptionThreadFunction_h

#include <vector>
#include "Reception.h"
#include "Doctor.h"
#include <memory>
#include <thread>
#include <atomic>

class receptionThreadFunction
{
  private:
    Reception *reception;
    std::vector<std::shared_ptr<Doctor>> doctors;

  public:
    receptionThreadFunction(Reception *reception, std::vector<std::shared_ptr<Doctor>> &doctors);
    void operator()(std::atomic<bool> &end);
};
#endif