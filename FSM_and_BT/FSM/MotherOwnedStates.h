#ifndef MOTHER_OWNED_STATES_H
#define MOTHER_OWNED_STATES_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  MotherOwnedStates.h [FSM] : 어머니 체첵의 상태들
//------------------------------------------------------------------------
#include "State.h"

class Mother;

//전역 상태 : "야크 데려왔어" -> 젖 짜기, "낑낑" -> 밥 준비, "아롤 다 말랐다" -> 아롤 +1
class MotherGlobalState : public State<Mother>
{
public:
  static MotherGlobalState* Instance();
  bool OnMessage(Mother* mother, const Telegram& msg) override;
};

//게르 살림 (시작 상태) : 메시지가 올 때까지 계속한다
class DoGerChores : public State<Mother>
{
public:
  static DoGerChores* Instance();
  void Execute(Mother* mother) override;
};

//젖 짜기 : 우유가 3이 되면 아롤 만들기로
class MilkYaks : public State<Mother>
{
public:
  static MilkYaks* Instance();
  void Execute(Mother* mother) override;
};

//아롤 만들기 : 지붕에 널고 "아롤 다 말랐다"를 자신에게 예약한 뒤 게르 살림으로
class MakeAaruul : public State<Mother>
{
public:
  static MakeAaruul* Instance();
  void Execute(Mother* mother) override;
};

//반하르 밥 준비하기 : 상태 블립. 2턴 뒤 "밥 먹어라"를 보내고 하던 일로 돌아간다
class PrepareDogFood : public State<Mother>
{
public:
  static PrepareDogFood* Instance();
  void Enter(Mother* mother) override;
  void Execute(Mother* mother) override;
};

#endif
