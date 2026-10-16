#ifndef CRUDETIMER_H
#define CRUDETIMER_H

#include <chrono>

#define Clock CrudeTimer::Instance()

class CrudeTimer
{
private:

  std::chrono::steady_clock::time_point m_StartTime;

  CrudeTimer():m_StartTime(std::chrono::steady_clock::now()){}

public:

  static CrudeTimer* Instance();

  double GetCurrentTime()
  {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - m_StartTime).count();
  }
};

#endif
