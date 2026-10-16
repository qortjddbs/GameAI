#include "Bankhar.h"
#include "BankharBehaviors.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

Bankhar::Bankhar(int id):BaseGameEntity(id),
                         m_pBehaviorTree(CreateBankharBehaviorTree()),
                         m_iHunger(0),
                         m_bFoodArrived(false),
                         m_bBegged(false)
{
}

Bankhar::~Bankhar()
{
  delete m_pBehaviorTree;
}

void Bankhar::Update()
{
  m_pBehaviorTree->Tick(this);
}

//메시지를 받으면 바로 행동하지 않고 메모만 남긴다. 다음 턴에 트리의 2순위 질문이 읽는다.
bool Bankhar::HandleMessage(const Telegram& msg)
{
  if (msg.Msg == Msg_FoodReady)
  {
    cout << "\n" << GetNameOfEntity(ID()) << ": 멍! 밥이다! (밥 도착 메모)";

    m_bFoodArrived = true;

    return true;
  }

  return false;
}
