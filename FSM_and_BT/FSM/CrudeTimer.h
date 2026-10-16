#ifndef CRUDETIMER_H
#define CRUDETIMER_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  CrudeTimer.h : 프로그램 시작 후 흐른 실제 시간(초). 예약 메시지 시각 계산에 쓴다.
//                 (Buckland 원본은 Windows의 timeGetTime 사용 -> 표준 C++ chrono로 바꿈)
//------------------------------------------------------------------------
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
