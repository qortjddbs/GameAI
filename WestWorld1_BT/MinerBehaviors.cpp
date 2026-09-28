#include "MinerBehaviors.h"
#include "BehaviorTree.h"
#include "Miner.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

//define this to output to a file
#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

namespace
{
  void PrintMinerLine(Miner* pMiner, const char* message)
  {
    SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << message;
  }

  void PrintExitActivity(Miner* pMiner)
  {
    switch (pMiner->CurrentActivity())
    {
    case activity_mining:
      PrintMinerLine(pMiner, "Ah'm leavin' the goldmine with mah pockets full o' sweet gold");
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

  class DigForNugget : public BehaviorNode
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

  class DepositGold : public BehaviorNode
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_banking);

      pMiner->AddToWealth(pMiner->GoldCarried());
      pMiner->SetGoldCarried(0);

      SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
      cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
           << "Depositing gold. Total savings now: " << pMiner->Wealth();

      if (pMiner->Wealth() >= ComfortLevel)
      {
        SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
             << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      }

      return success;
    }
  };

  class SleepUntilRested : public BehaviorNode
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_sleeping);

      pMiner->DecreaseFatigue();

      PrintMinerLine(pMiner, "ZZZZ... ");

      if (!pMiner->Fatigued())
      {
        PrintMinerLine(pMiner, "What a God darn fantastic nap! Time to find more gold");
        return success;
      }

      return running;
    }
  };

  class DrinkWhiskey : public BehaviorNode
  {
  public:

    virtual BehaviorStatus Tick(Miner* pMiner)
    {
      EnterActivity(pMiner, activity_drinking);

      if (pMiner->Thirsty())
      {
        pMiner->BuyAndDrinkAWhiskey();
        PrintMinerLine(pMiner, "That's mighty fine sippin liquer");

        return success;
      }

      PrintMinerLine(pMiner, "ERROR! ERROR! ERROR!");

      return failure;
    }
  };

  SequenceNode* MakeSequence(BehaviorNode* condition, BehaviorNode* action)
  {
    SequenceNode* sequence = new SequenceNode;
    sequence->AddChild(condition);
    sequence->AddChild(action);

    return sequence;
  }
}

BehaviorNode* CreateMinerBehaviorTree()
{
  SelectorNode* root = new SelectorNode;

  root->AddChild(MakeSequence(new ConditionNode(ShouldRestAtHome), new SleepUntilRested));
  root->AddChild(MakeSequence(new ConditionNode(IsThirsty), new DrinkWhiskey));
  root->AddChild(MakeSequence(new ConditionNode(PocketsAreFull), new DepositGold));
  root->AddChild(new DigForNugget);

  return root;
}
