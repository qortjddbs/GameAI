#include "MotherBehaviors.h"
#include "Mother.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

namespace
{
  void Say(Mother* pMother, const char* line)
  {
    cout << "\n" << GetNameOfEntity(pMother->ID()) << ": " << line;
  }

  //---------------------------------------------------------- 질문

  bool DogIsHungry(Mother* pMother)
  {
    return pMother->DogHungry();
  }

  bool HasEnoughMilk(Mother* pMother)
  {
    return pMother->EnoughMilk();
  }

  bool YaksAreHome(Mother* pMother)
  {
    return pMother->YaksHome();
  }

  //---------------------------------------------------------- 행동

  class PrepareDogFood : public BehaviorNode<Mother>
  {
  public:
    BehaviorStatus Tick(Mother* pMother) override
    {
      pMother->PrepFood();

      cout << "\n" << GetNameOfEntity(pMother->ID()) << ": "
           << "반하르 밥으로 남은 고기를 손질한다 (" << pMother->FoodPrepCount() << "/" << FoodPrepTurns << ")";

      if (!pMother->FoodPrepDone())
      {
        return running;
      }

      Say(pMother, "반하르야, 밥 먹어라!");

      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, pMother->ID(), ent_Bankhar, Msg_FoodReady);

      pMother->StartFoodPrep();
      pMother->ClearDogHungry();

      return success;
    }
  };

  class MakeAaruul : public BehaviorNode<Mother>
  {
  public:
    BehaviorStatus Tick(Mother* pMother) override
    {
      Say(pMother, "우유를 끓이고 굳혀서 아롤을 만든 뒤 게르 지붕에 넌다");

      pMother->ResetMilk();

      //아롤이 다 마르면 알 수 있도록 나 자신에게 예약 메시지를 건다
      Dispatch->DispatchMessage(AaruulDryDelay, pMother->ID(), pMother->ID(), Msg_AaruulDry);

      return success;
    }
  };

  class MilkYaks : public BehaviorNode<Mother>
  {
  public:
    BehaviorStatus Tick(Mother* pMother) override
    {
      pMother->AddMilk();

      cout << "\n" << GetNameOfEntity(pMother->ID()) << ": "
           << "야크 젖을 짠다 (우유 " << pMother->Milk() << "/" << MilkForAaruul << ")";

      if (!pMother->EnoughMilk())
      {
        return running;
      }

      //젖 짜기 끝 : 메모를 지운다 (다음 턴엔 2순위 "우유 3 이상?"이 아롤 만들기를 고른다)
      pMother->ClearYaksHome();

      return success;
    }
  };

  class DoGerChores : public BehaviorNode<Mother>
  {
  public:
    BehaviorStatus Tick(Mother* pMother) override
    {
      Say(pMother, "수테차를 끓이며 게르 살림을 한다");

      return success;
    }
  };
}

BehaviorNode<Mother>* CreateMotherBehaviorTree()
{
  SelectorNode<Mother>* root = new SelectorNode<Mother>;

  root->AddChild(MakeSequence(DogIsHungry,   new PrepareDogFood));  //1순위
  root->AddChild(MakeSequence(HasEnoughMilk, new MakeAaruul));      //2순위
  root->AddChild(MakeSequence(YaksAreHome,   new MilkYaks));        //3순위
  root->AddChild(new DoGerChores);                                  //4순위

  return root;
}
