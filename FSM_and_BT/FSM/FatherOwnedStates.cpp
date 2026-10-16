//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  FatherOwnedStates.cpp [FSM]
//------------------------------------------------------------------------
#include "FatherOwnedStates.h"
#include "Father.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

static void Say(Father* pFather, const char* line)
{
  cout << "\n" << GetNameOfEntity(pFather->ID()) << ": " << line;
}

//------------------------------------------------------------ FatherGlobalState

FatherGlobalState* FatherGlobalState::Instance()
{
  static FatherGlobalState instance;

  return &instance;
}

bool FatherGlobalState::OnMessage(Father* pFather, const Telegram& msg)
{
  if (msg.Msg == Msg_WolfSpotted)
  {
    //지금 무슨 일을 하고 있든 늑대부터 쫓는다
    pFather->GetFSM()->ChangeState(ChaseWolf::Instance());

    return true;
  }

  return false;
}

//------------------------------------------------------------ RestInGer

RestInGer* RestInGer::Instance()
{
  static RestInGer instance;

  return &instance;
}

void RestInGer::Enter(Father* pFather)
{
  //초원에서 돌아올 때만 체첵에게 알린다 (늑대 쫓기에서 복귀할 때는 이미 게르에 있음)
  if (pFather->Location() == pasture)
  {
    Say(pFather, "야크들이 배불리 먹었다. 게르로 데려간다");

    pFather->ChangeLocation(ger);
    pFather->ResetYakFullness();

    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pFather->ID(), ent_Mother_Tsetseg, Msg_YaksHome);
  }
}

void RestInGer::Execute(Father* pFather)
{
  if (pFather->Tired())
  {
    pFather->DecreaseFatigue();

    cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
         << "게르에서 수테차를 마시며 쉰다 (피로도 " << pFather->Fatigue() << ")";
  }

  if (!pFather->Tired())
  {
    Say(pFather, "푹 쉬었다. 야크들 풀 먹이러 가야지");

    pFather->GetFSM()->ChangeState(GrazeYaks::Instance());
  }
}

//------------------------------------------------------------ GrazeYaks

GrazeYaks* GrazeYaks::Instance()
{
  static GrazeYaks instance;

  return &instance;
}

void GrazeYaks::Enter(Father* pFather)
{
  if (pFather->Location() == ger)
  {
    Say(pFather, "야크 떼를 몰고 초원으로 나간다");

    pFather->ChangeLocation(pasture);
  }
}

void GrazeYaks::Execute(Father* pFather)
{
  pFather->FeedYaks();
  pFather->IncreaseFatigue(1);

  cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
       << "야크들이 풀을 뜯는 걸 지켜본다 (배부름 " << pFather->YakFullness()
       << "/" << YakFullLevel << ", 피로도 " << pFather->Fatigue() << ")";

  if (pFather->YaksFull())
  {
    pFather->GetFSM()->ChangeState(RestInGer::Instance());
  }
}

//------------------------------------------------------------ ChaseWolf

ChaseWolf* ChaseWolf::Instance()
{
  static ChaseWolf instance;

  return &instance;
}

void ChaseWolf::Enter(Father* pFather)
{
  Say(pFather, "늑대라고?! 몽둥이를 들고 달려간다");
}

void ChaseWolf::Execute(Father* pFather)
{
  pFather->IncreaseFatigue(2);

  cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
       << "늑대를 쫓아냈다! (피로도 " << pFather->Fatigue() << ")";

  //상태 블립 : 직전 상태(하던 일)로 돌아간다
  pFather->GetFSM()->RevertToPreviousState();
}

void ChaseWolf::Exit(Father* pFather)
{
  Say(pFather, "하던 일로 돌아간다");
}
