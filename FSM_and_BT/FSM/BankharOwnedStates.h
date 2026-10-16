#ifndef BANKHAR_OWNED_STATES_H
#define BANKHAR_OWNED_STATES_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  BankharOwnedStates.h [FSM] : 경비견 반하르의 상태들
//------------------------------------------------------------------------
#include "State.h"

class Bankhar;

//전역 상태 : 지금 무슨 상태든 매 턴 10% 확률로 늑대 냄새 -> 짖어서 경고
class BankharGlobalState : public State<Bankhar>
{
public:
  static BankharGlobalState* Instance();
  void Execute(Bankhar* dog) override;
};

//게르 지키기 (시작 상태) : 배고픔이 6이 되면 밥 조르기로
class GuardGer : public State<Bankhar>
{
public:
  static GuardGer* Instance();
  void Execute(Bankhar* dog) override;
};

//밥 조르기 : 들어갈 때 "낑낑", "밥 먹어라"를 받으면 밥 먹기로
class BegForFood : public State<Bankhar>
{
public:
  static BegForFood* Instance();
  void Enter(Bankhar* dog) override;
  void Execute(Bankhar* dog) override;
  bool OnMessage(Bankhar* dog, const Telegram& msg) override;
};

//밥 먹기 : 배고픔을 0으로 하고 게르 지키기로
class EatFood : public State<Bankhar>
{
public:
  static EatFood* Instance();
  void Execute(Bankhar* dog) override;
};

//짖어서 경고 : 상태 블립. 들어갈 때 "늑대다!", 한 번 짖고 하던 일로 돌아간다
class BarkWarning : public State<Bankhar>
{
public:
  static BarkWarning* Instance();
  void Enter(Bankhar* dog) override;
  void Execute(Bankhar* dog) override;
};

#endif
