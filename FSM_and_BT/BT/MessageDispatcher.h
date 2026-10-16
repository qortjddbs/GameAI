#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H

#include <set>

#include "Telegram.h"

class BaseGameEntity;

const double SEND_MSG_IMMEDIATELY = 0.0;

#define Dispatch MessageDispatcher::Instance()

class MessageDispatcher
{
private:

  //예약 메시지 우편함 (배달 시각 순으로 자동 정렬)
  std::set<Telegram> PriorityQ;

  void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

public:

  static MessageDispatcher* Instance();

  //delay가 0이면 바로 전달하고, 0보다 크면 우편함에 보관한다
  void DispatchMessage(double delay, int sender, int receiver, int msg);

  //매 턴 호출 : 배달 시각이 지난 예약 메시지를 전달한다
  void DispatchDelayedMessages();
};

#endif
