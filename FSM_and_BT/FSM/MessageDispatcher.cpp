//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  MessageDispatcher.cpp
//------------------------------------------------------------------------
#include "MessageDispatcher.h"
#include "BaseGameEntity.h"
#include "CrudeTimer.h"
#include "EntityManager.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance;

  return &instance;
}

void MessageDispatcher::Discharge(BaseGameEntity* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    cout << "\n[메시지 처리 안 됨] " << GetNameOfEntity(pReceiver->ID())
         << " 이(가) '" << MsgToStr(telegram.Msg) << "' 메시지를 처리하지 못했다";
  }
}

void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg)
{
  Telegram telegram(0, sender, receiver, msg);

  if (delay <= 0.0)
  {
    cout << "\n[즉시 메시지] " << Clock->GetCurrentTime() << "초 : "
         << GetNameOfEntity(sender) << " -> " << GetNameOfEntity(receiver) << " : " << MsgToStr(msg);

    Discharge(EntityMgr->GetEntityFromID(receiver), telegram);
  }
  else
  {
    telegram.DispatchTime = Clock->GetCurrentTime() + delay;

    PriorityQ.insert(telegram);

    cout << "\n[예약 메시지 등록] " << Clock->GetCurrentTime() << "초 : "
         << GetNameOfEntity(sender) << " -> " << GetNameOfEntity(receiver) << " : " << MsgToStr(msg)
         << " (" << delay << "초 뒤 도착)";
  }
}

void MessageDispatcher::DispatchDelayedMessages()
{
  double CurrentTime = Clock->GetCurrentTime();

  while (!PriorityQ.empty() && PriorityQ.begin()->DispatchTime < CurrentTime)
  {
    const Telegram& telegram = *PriorityQ.begin();

    cout << "\n[예약 메시지 도착] " << CurrentTime << "초 : "
         << GetNameOfEntity(telegram.Receiver) << " 에게 : " << MsgToStr(telegram.Msg);

    Discharge(EntityMgr->GetEntityFromID(telegram.Receiver), telegram);

    PriorityQ.erase(PriorityQ.begin());
  }
}
