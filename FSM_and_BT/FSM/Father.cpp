//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Father.cpp [FSM]
//------------------------------------------------------------------------
#include "Father.h"
#include "FatherOwnedStates.h"

Father::Father(int id):BaseGameEntity(id),
                       m_StateMachine(this, RestInGer::Instance(), FatherGlobalState::Instance()),
                       m_Location(ger),
                       m_iFatigue(3),
                       m_iYakFullness(0)
{
}

void Father::Update()
{
  m_StateMachine.Update();
}

bool Father::HandleMessage(const Telegram& msg)
{
  return m_StateMachine.HandleMessage(msg);
}
