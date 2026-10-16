//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  CrudeTimer.cpp
//------------------------------------------------------------------------
#include "CrudeTimer.h"

CrudeTimer* CrudeTimer::Instance()
{
  static CrudeTimer instance;

  return &instance;
}
