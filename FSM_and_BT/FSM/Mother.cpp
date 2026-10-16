//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Mother.cpp [FSM]
//------------------------------------------------------------------------
#include "Mother.h"
#include "MotherOwnedStates.h"

Mother::Mother(int id):BaseGameEntity(id),
                       m_StateMachine(this, DoGerChores::Instance(), MotherGlobalState::Instance()),
                       m_iMilk(0),
                       m_iAaruul(0),
                       m_iFoodPrepCount(0)
{
}

void Mother::Update()
{
  m_StateMachine.Update();
}

bool Mother::HandleMessage(const Telegram& msg)
{
  return m_StateMachine.HandleMessage(msg);
}
