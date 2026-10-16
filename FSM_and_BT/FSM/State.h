#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  State.h : 상태 하나의 틀 (Buckland 원본 기반)
//            Enter(들어갈 때 1번) / Execute(머무는 동안 매 턴) / Exit(나갈 때 1번) /
//            OnMessage(이 상태일 때 메시지가 오면). 쓰지 않는 함수는 아무것도 하지 않는다.
//------------------------------------------------------------------------

struct Telegram;

template <class entity_type>
class State
{
public:

  virtual ~State(){}

  virtual void Enter(entity_type*){}

  virtual void Execute(entity_type*){}

  virtual void Exit(entity_type*){}

  virtual bool OnMessage(entity_type*, const Telegram&){return false;}
};

#endif
