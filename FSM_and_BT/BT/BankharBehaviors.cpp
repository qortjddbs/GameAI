#include "BankharBehaviors.h"
#include "Bankhar.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <cstdlib>
#include <iostream>
using std::cout;

namespace
{
  void Say(Bankhar* pDog, const char* line)
  {
    cout << "\n" << GetNameOfEntity(pDog->ID()) << ": " << line;
  }

  //---------------------------------------------------------- 질문

  //FSM 전역 상태의 Execute 와 같은 역할 : 매 턴 맨 먼저 확인한다
  bool SmellsWolf(Bankhar* pDog)
  {
    return rand() / (RAND_MAX + 1.0) < WolfChance;
  }

  bool HasFoodArrived(Bankhar* pDog)
  {
    return pDog->FoodArrived();
  }

  bool IsHungry(Bankhar* pDog)
  {
    return pDog->Hungry();
  }

  //---------------------------------------------------------- 행동

  class BarkWarning : public BehaviorNode<Bankhar>
  {
  public:
    BehaviorStatus Tick(Bankhar* pDog) override
    {
      Say(pDog, "킁킁... 늑대 냄새다! 멍멍멍!");

      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pDog->ID(), ent_Father_Bat, Msg_WolfSpotted);

      return success;
    }
  };

  class EatFood : public BehaviorNode<Bankhar>
  {
  public:
    BehaviorStatus Tick(Bankhar* pDog) override
    {
      pDog->ResetHunger();
      pDog->ClearFoodArrived();
      pDog->SetBegged(false);

      Say(pDog, "허겁지겁 밥을 먹는다");

      return success;
    }
  };

  class BegForFood : public BehaviorNode<Bankhar>
  {
  public:
    BehaviorStatus Tick(Bankhar* pDog) override
    {
      if (!pDog->Begged())
      {
        Say(pDog, "낑낑... 체첵에게 밥을 조른다");

        pDog->SetBegged(true);

        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pDog->ID(), ent_Mother_Tsetseg, Msg_DogHungry);
      }
      else
      {
        cout << "\n" << GetNameOfEntity(pDog->ID()) << ": "
             << "밥그릇 앞에서 꼬리를 흔들며 기다린다 (배고픔 " << pDog->Hunger() << ")";
      }

      return running;
    }
  };

  class GuardGer : public BehaviorNode<Bankhar>
  {
  public:
    BehaviorStatus Tick(Bankhar* pDog) override
    {
      pDog->IncreaseHunger();

      cout << "\n" << GetNameOfEntity(pDog->ID()) << ": "
           << "게르 앞을 지킨다 (배고픔 " << pDog->Hunger() << ")";

      return success;
    }
  };
}

BehaviorNode<Bankhar>* CreateBankharBehaviorTree()
{
  SelectorNode<Bankhar>* root = new SelectorNode<Bankhar>;

  root->AddChild(MakeSequence(SmellsWolf,     new BarkWarning));  //1순위
  root->AddChild(MakeSequence(HasFoodArrived, new EatFood));      //2순위
  root->AddChild(MakeSequence(IsHungry,       new BegForFood));   //3순위
  root->AddChild(new GuardGer);                                   //4순위

  return root;
}
