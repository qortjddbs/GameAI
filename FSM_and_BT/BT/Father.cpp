#include "Father.h"
#include "FatherBehaviors.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

Father::Father(int id):BaseGameEntity(id),
                       m_pBehaviorTree(CreateFatherBehaviorTree()),
                       m_Location(ger),
                       m_iFatigue(3),
                       m_iYakFullness(0),
                       m_bWolfAlert(false)
{
}

Father::~Father()
{
  delete m_pBehaviorTree;
}

void Father::Update()
{
  m_pBehaviorTree->Tick(this);
}

//메시지를 받으면 바로 행동하지 않고 메모만 남긴다. 다음 턴에 트리의 1순위 질문이 읽는다.
bool Father::HandleMessage(const Telegram& msg)
{
  if (msg.Msg == Msg_WolfSpotted)
  {
    cout << "\n" << GetNameOfEntity(ID()) << ": 늑대라고?! (늑대 경보 메모)";

    m_bWolfAlert = true;

    return true;
  }

  return false;
}
