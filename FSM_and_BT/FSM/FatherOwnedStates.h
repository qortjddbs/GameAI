#ifndef FATHER_OWNED_STATES_H
#define FATHER_OWNED_STATES_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  FatherOwnedStates.h [FSM] : 아버지 바트의 상태들
//------------------------------------------------------------------------
#include "State.h"

class Father;

//전역 상태 : "늑대다!"를 어느 상태에서든 받아 늑대 쫓기로 보낸다
class FatherGlobalState : public State<Father>
{
public:
  static FatherGlobalState* Instance();
  bool OnMessage(Father* father, const Telegram& msg) override;
};

//게르에서 쉬기 (시작 상태) : 피로도가 0이 되면 방목하기로
class RestInGer : public State<Father>
{
public:
  static RestInGer* Instance();
  void Enter(Father* father) override;
  void Execute(Father* father) override;
};

//방목하기 : 야크 배부름이 10이 되면 게르에서 쉬기로
class GrazeYaks : public State<Father>
{
public:
  static GrazeYaks* Instance();
  void Enter(Father* father) override;
  void Execute(Father* father) override;
};

//늑대 쫓기 : 상태 블립. 한 번 쫓고 하던 일로 돌아간다
class ChaseWolf : public State<Father>
{
public:
  static ChaseWolf* Instance();
  void Enter(Father* father) override;
  void Execute(Father* father) override;
  void Exit(Father* father) override;
};

#endif
