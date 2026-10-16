#include "Mother.h"
#include "MotherBehaviors.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

Mother::Mother(int id):BaseGameEntity(id),
                       m_pBehaviorTree(CreateMotherBehaviorTree()),
                       m_iMilk(0),
                       m_iAaruul(0),
                       m_iFoodPrepCount(0),
                       m_bYaksHome(false),
                       m_bDogHungry(false)
{
}

Mother::~Mother()
{
  delete m_pBehaviorTree;
}

void Mother::Update()
{
  m_pBehaviorTree->Tick(this);
}

//메시지를 받으면 바로 행동하지 않고 메모만 남긴다. 다음 턴에 트리 위쪽의 질문이 읽는다.
bool Mother::HandleMessage(const Telegram& msg)
{
  switch(msg.Msg)
  {
  case Msg_YaksHome:

    cout << "\n" << GetNameOfEntity(ID()) << ": 어서 와요. 젖 짜러 갈게요 (야크 귀가 메모)";

    m_bYaksHome = true;

    return true;

  case Msg_DogHungry:

    cout << "\n" << GetNameOfEntity(ID()) << ": 반하르가 배고픈가 보네 (반하르 배고픔 메모)";

    m_bDogHungry = true;

    return true;

  case Msg_AaruulDry:

    //메모 없이 받는 즉시 처리 (FSM 에서도 상태를 바꾸지 않는 메시지)
    m_iAaruul += 1;

    cout << "\n" << GetNameOfEntity(ID()) << ": 아롤이 다 말랐다. 걷어서 보관한다 (아롤 " << m_iAaruul << "개)";

    return true;
  }

  return false;
}
