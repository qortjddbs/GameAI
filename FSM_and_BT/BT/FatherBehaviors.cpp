#include "FatherBehaviors.h"
#include "Father.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

namespace
{
  void Say(Father* pFather, const char* line)
  {
    cout << "\n" << GetNameOfEntity(pFather->ID()) << ": " << line;
  }

  //---------------------------------------------------------- 질문

  bool HasWolfAlert(Father* pFather)
  {
    return pFather->WolfAlert();
  }

  //초원에 있을 때는 피곤해도 쉬지 않는다 (방목을 끝까지 한다)
  bool ShouldRestInGer(Father* pFather)
  {
    return pFather->Location() == ger && pFather->Tired();
  }

  //---------------------------------------------------------- 행동

  class ChaseWolf : public BehaviorNode<Father>
  {
  public:
    BehaviorStatus Tick(Father* pFather) override
    {
      pFather->IncreaseFatigue(2);

      cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
           << "몽둥이를 들고 달려가 늑대를 쫓아냈다! (피로도 " << pFather->Fatigue() << ")";

      //메모를 지운다 (안 지우면 매 턴 계속 늑대를 쫓는다)
      pFather->ClearWolfAlert();

      return success;
    }
  };

  class RestInGer : public BehaviorNode<Father>
  {
  public:
    BehaviorStatus Tick(Father* pFather) override
    {
      pFather->DecreaseFatigue();

      cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
           << "게르에서 수테차를 마시며 쉰다 (피로도 " << pFather->Fatigue() << ")";

      if (!pFather->Tired())
      {
        Say(pFather, "푹 쉬었다. 야크들 풀 먹이러 가야지");
      }

      return success;
    }
  };

  class GrazeYaks : public BehaviorNode<Father>
  {
  public:
    BehaviorStatus Tick(Father* pFather) override
    {
      if (pFather->Location() == ger)
      {
        Say(pFather, "야크 떼를 몰고 초원으로 나간다");

        pFather->ChangeLocation(pasture);
      }

      pFather->FeedYaks();
      pFather->IncreaseFatigue(1);

      cout << "\n" << GetNameOfEntity(pFather->ID()) << ": "
           << "야크들이 풀을 뜯는 걸 지켜본다 (배부름 " << pFather->YakFullness()
           << "/" << YakFullLevel << ", 피로도 " << pFather->Fatigue() << ")";

      if (!pFather->YaksFull())
      {
        return running;
      }

      Say(pFather, "야크들이 배불리 먹었다. 게르로 데려간다");

      pFather->ChangeLocation(ger);
      pFather->ResetYakFullness();

      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pFather->ID(), ent_Mother_Tsetseg, Msg_YaksHome);

      return success;
    }
  };
}

BehaviorNode<Father>* CreateFatherBehaviorTree()
{
  SelectorNode<Father>* root = new SelectorNode<Father>;

  root->AddChild(MakeSequence(HasWolfAlert,    new ChaseWolf));  //1순위
  root->AddChild(MakeSequence(ShouldRestInGer, new RestInGer));  //2순위
  root->AddChild(new GrazeYaks);                                 //3순위

  return root;
}
