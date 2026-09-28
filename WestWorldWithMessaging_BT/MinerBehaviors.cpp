#include "MinerBehaviors.h"
#include "Miner.h"
#include "Locations.h"
#include "EntityNames.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

namespace
{
  void PrintMinerLine(Miner* pMiner, const char* message)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << message;
  }

  void PrintExitActivity(Miner* pMiner)
  {
    switch (pMiner->CurrentActivity())
    {
    case activity_mining:
      PrintMinerLine(pMiner, "Ah'm leavin' the goldmine");
      break;

    case activity_banking:
      PrintMinerLine(pMiner, "Leavin' the bank");
      break;

    case activity_sleeping:
      PrintMinerLine(pMiner, "Leaving the house");
      break;

    case activity_drinking:
      PrintMinerLine(pMiner, "Leaving the saloon, feelin' good");
      break;

    default:
      break;
    }
  }

  void EnterActivity(Miner* pMiner, miner_activity newActivity)
  {
    if (pMiner->CurrentActivity() == newActivity)
    {
      return;
    }

    PrintExitActivity(pMiner);
    pMiner->ChangeActivity(newActivity);

    switch (newActivity)
    {
    case activity_mining:
      if (pMiner->Location() != goldmine)
      {
        PrintMinerLine(pMiner, "Walkin' to the goldmine");
        pMiner->ChangeLocation(goldmine);
      }
      break;

    case activity_banking:
      if (pMiner->Location() != bank)
      {
        PrintMinerLine(pMiner, "Goin' to the bank. Yes siree");
        pMiner->ChangeLocation(bank);
      }
      break;

    case activity_sleeping:
      if (pMiner->Location() != shack)
      {
        PrintMinerLine(pMiner, "Walkin' home");
        pMiner->ChangeLocation(shack);

        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
                                  pMiner->ID(),
                                  ent_Elsa,
                                  Msg_HiHoneyImHome,
                                  NO_ADDITIONAL_INFO);
      }
      break;

    case activity_drinking:
      if (pMiner->Location() != saloon)
      {
        pMiner->ChangeLocation(saloon);
        PrintMinerLine(pMiner, "Boy, ah sure is thusty! Walking to the saloon");
      }
      break;

    default:
      break;
    }
  }

  bool HasStewReady(Miner* pMiner)
  {
    return pMiner->StewReady();
  }

  bool IsThirsty(Miner* pMiner)
  {
    return pMiner->Thirsty();
  }

  bool ShouldRestAtHome(Miner* pMiner)
  {
    return pMiner->Wealth() >= ComfortLevel && pMiner->Fatigued();
  }

  bool PocketsAreFull(Miner* pMiner)
  {
    return pMiner->PocketsFull();
  }

  class EatStew : public BehaviorNode<Miner>
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      PrintMinerLine(pMiner, "Smells Reaaal goood Elsa!");
      PrintMinerLine(pMiner, "Tastes real good too!");
      PrintMinerLine(pMiner, "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'");
      pMiner->ClearStewReady();

      return success;
    }
  };

  class DigForNugget : public BehaviorNode<Miner>
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_mining);

      pMiner->AddToGoldCarried(1);
      pMiner->IncreaseFatigue();

      PrintMinerLine(pMiner, "Pickin' up a nugget");

      return success;
    }
  };

  class DepositGold : public BehaviorNode<Miner>
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_banking);

      pMiner->AddToWealth(pMiner->GoldCarried());
      pMiner->SetGoldCarried(0);

      cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
           << "Depositing gold. Total savings now: " << pMiner->Wealth();

      if (pMiner->Wealth() >= ComfortLevel)
      {
        cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
             << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      }

      return success;
    }
  };

  class SleepUntilRested : public BehaviorNode<Miner>
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_sleeping);

      if (!pMiner->Fatigued())
      {
        PrintMinerLine(pMiner, "All mah fatigue has drained away. Time to find more gold!");

        return success;
      }

      pMiner->DecreaseFatigue();
      PrintMinerLine(pMiner, "ZZZZ... ");

      return running;
    }
  };

  class DrinkWhiskey : public BehaviorNode<Miner>
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_drinking);

      if (pMiner->Thirsty())
      {
        pMiner->BuyAndDrinkAWhiskey();
        PrintMinerLine(pMiner, "That's mighty fine sippin' liquer");

        return success;
      }

      cout << "\nERROR!\nERROR!\nERROR!";

      return failure;
    }
  };

  SequenceNode<Miner>* MakeSequence(BehaviorNode<Miner>* condition, BehaviorNode<Miner>* action)
  {
    SequenceNode<Miner>* sequence = new SequenceNode<Miner>;
    sequence->AddChild(condition);
    sequence->AddChild(action);

    return sequence;
  }
}

BehaviorNode<Miner>* CreateMinerBehaviorTree()
{
  SelectorNode<Miner>* root = new SelectorNode<Miner>;

  root->AddChild(MakeSequence(new ConditionNode<Miner>(HasStewReady), new EatStew));
  root->AddChild(MakeSequence(new ConditionNode<Miner>(ShouldRestAtHome), new SleepUntilRested));
  root->AddChild(MakeSequence(new ConditionNode<Miner>(IsThirsty), new DrinkWhiskey));
  root->AddChild(MakeSequence(new ConditionNode<Miner>(PocketsAreFull), new DepositGold));
  root->AddChild(new DigForNugget);

  return root;
}
