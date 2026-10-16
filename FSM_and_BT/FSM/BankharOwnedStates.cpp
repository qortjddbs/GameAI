//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  BankharOwnedStates.cpp [FSM]
//------------------------------------------------------------------------
#include "BankharOwnedStates.h"
#include "Bankhar.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <cstdlib>
#include <iostream>
using std::cout;

static void Say(Bankhar* pDog, const char* line)
{
  cout << "\n" << GetNameOfEntity(pDog->ID()) << ": " << line;
}

//------------------------------------------------------------ BankharGlobalState

BankharGlobalState* BankharGlobalState::Instance()
{
  static BankharGlobalState instance;

  return &instance;
}

void BankharGlobalState::Execute(Bankhar* pDog)
{
  if (rand() / (RAND_MAX + 1.0) < WolfChance)
  {
    pDog->GetFSM()->ChangeState(BarkWarning::Instance());
  }
}

//------------------------------------------------------------ GuardGer

GuardGer* GuardGer::Instance()
{
  static GuardGer instance;

  return &instance;
}

void GuardGer::Execute(Bankhar* pDog)
{
  if (pDog->Hungry())
  {
    pDog->GetFSM()->ChangeState(BegForFood::Instance());
  }
  else
  {
    pDog->IncreaseHunger();

    cout << "\n" << GetNameOfEntity(pDog->ID()) << ": "
         << "게르 앞을 지킨다 (배고픔 " << pDog->Hunger() << ")";
  }
}

//------------------------------------------------------------ BegForFood

BegForFood* BegForFood::Instance()
{
  static BegForFood instance;

  return &instance;
}

void BegForFood::Enter(Bankhar* pDog)
{
  //짖기에 다녀와서 다시 들어온 경우에는 또 조르지 않는다 (Bankhar.h 의 m_bBegged 설명 참고)
  if (pDog->Begged())
  {
    return;
  }

  Say(pDog, "낑낑... 체첵에게 밥을 조른다");

  pDog->SetBegged(true);

  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pDog->ID(), ent_Mother_Tsetseg, Msg_DogHungry);
}

void BegForFood::Execute(Bankhar* pDog)
{
  cout << "\n" << GetNameOfEntity(pDog->ID()) << ": "
       << "밥그릇 앞에서 꼬리를 흔들며 기다린다 (배고픔 " << pDog->Hunger() << ")";
}

bool BegForFood::OnMessage(Bankhar* pDog, const Telegram& msg)
{
  if (msg.Msg == Msg_FoodReady)
  {
    Say(pDog, "멍! 밥이다!");

    pDog->GetFSM()->ChangeState(EatFood::Instance());

    return true;
  }

  return false;
}

//------------------------------------------------------------ EatFood

EatFood* EatFood::Instance()
{
  static EatFood instance;

  return &instance;
}

void EatFood::Execute(Bankhar* pDog)
{
  pDog->ResetHunger();
  pDog->SetBegged(false);

  Say(pDog, "허겁지겁 밥을 먹는다");

  pDog->GetFSM()->ChangeState(GuardGer::Instance());
}

//------------------------------------------------------------ BarkWarning

BarkWarning* BarkWarning::Instance()
{
  static BarkWarning instance;

  return &instance;
}

void BarkWarning::Enter(Bankhar* pDog)
{
  Say(pDog, "킁킁... 늑대 냄새다! 멍멍멍!");

  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pDog->ID(), ent_Father_Bat, Msg_WolfSpotted);
}

void BarkWarning::Execute(Bankhar* pDog)
{
  Say(pDog, "늑대가 물러날 때까지 짖는다");

  //상태 블립 : 직전 상태(하던 일)로 돌아간다
  pDog->GetFSM()->RevertToPreviousState();
}
