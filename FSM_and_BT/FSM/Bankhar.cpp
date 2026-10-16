//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Bankhar.cpp [FSM]
//------------------------------------------------------------------------
#include "Bankhar.h"
#include "BankharOwnedStates.h"

Bankhar::Bankhar(int id):BaseGameEntity(id),
                         m_StateMachine(this, GuardGer::Instance(), BankharGlobalState::Instance()),
                         m_iHunger(0),
                         m_bBegged(false)
{
}

void Bankhar::Update()
{
  m_StateMachine.Update();
}

bool Bankhar::HandleMessage(const Telegram& msg)
{
  return m_StateMachine.HandleMessage(msg);
}
