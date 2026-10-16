//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  MotherOwnedStates.cpp [FSM]
//------------------------------------------------------------------------
#include "MotherOwnedStates.h"
#include "Mother.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

static void Say(Mother* pMother, const char* line)
{
  cout << "\n" << GetNameOfEntity(pMother->ID()) << ": " << line;
}

//------------------------------------------------------------ MotherGlobalState

MotherGlobalState* MotherGlobalState::Instance()
{
  static MotherGlobalState instance;

  return &instance;
}

bool MotherGlobalState::OnMessage(Mother* pMother, const Telegram& msg)
{
  switch(msg.Msg)
  {
  case Msg_YaksHome:

    Say(pMother, "어서 와요. 젖 짜러 갈게요");

    pMother->GetFSM()->ChangeState(MilkYaks::Instance());

    return true;

  case Msg_DogHungry:

    Say(pMother, "반하르가 배고픈가 보네");

    pMother->GetFSM()->ChangeState(PrepareDogFood::Instance());

    return true;

  case Msg_AaruulDry:

    //상태는 바꾸지 않는다
    pMother->AddAaruul();

    cout << "\n" << GetNameOfEntity(pMother->ID()) << ": "
         << "아롤이 다 말랐다. 걷어서 보관한다 (아롤 " << pMother->Aaruul() << "개)";

    return true;
  }

  return false;
}

//------------------------------------------------------------ DoGerChores

DoGerChores* DoGerChores::Instance()
{
  static DoGerChores instance;

  return &instance;
}

void DoGerChores::Execute(Mother* pMother)
{
  Say(pMother, "수테차를 끓이며 게르 살림을 한다");
}

//------------------------------------------------------------ MilkYaks

MilkYaks* MilkYaks::Instance()
{
  static MilkYaks instance;

  return &instance;
}

void MilkYaks::Execute(Mother* pMother)
{
  pMother->AddMilk();

  cout << "\n" << GetNameOfEntity(pMother->ID()) << ": "
       << "야크 젖을 짠다 (우유 " << pMother->Milk() << "/" << MilkForAaruul << ")";

  if (pMother->EnoughMilk())
  {
    pMother->GetFSM()->ChangeState(MakeAaruul::Instance());
  }
}

//------------------------------------------------------------ MakeAaruul

MakeAaruul* MakeAaruul::Instance()
{
  static MakeAaruul instance;

  return &instance;
}

void MakeAaruul::Execute(Mother* pMother)
{
  Say(pMother, "우유를 끓이고 굳혀서 아롤을 만든 뒤 게르 지붕에 넌다");

  pMother->ResetMilk();

  //아롤이 다 마르면 알 수 있도록 나 자신에게 예약 메시지를 건다
  Dispatch->DispatchMessage(AaruulDryDelay, pMother->ID(), pMother->ID(), Msg_AaruulDry);

  pMother->GetFSM()->ChangeState(DoGerChores::Instance());
}

//------------------------------------------------------------ PrepareDogFood

PrepareDogFood* PrepareDogFood::Instance()
{
  static PrepareDogFood instance;

  return &instance;
}

void PrepareDogFood::Enter(Mother* pMother)
{
  pMother->StartFoodPrep();
}

void PrepareDogFood::Execute(Mother* pMother)
{
  pMother->PrepFood();

  cout << "\n" << GetNameOfEntity(pMother->ID()) << ": "
       << "반하르 밥으로 남은 고기를 손질한다 (" << pMother->FoodPrepCount() << "/" << FoodPrepTurns << ")";

  if (pMother->FoodPrepDone())
  {
    Say(pMother, "반하르야, 밥 먹어라!");

    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pMother->ID(), ent_Bankhar, Msg_FoodReady);

    //상태 블립 : 직전 상태(하던 일)로 돌아간다
    pMother->GetFSM()->RevertToPreviousState();
  }
}
