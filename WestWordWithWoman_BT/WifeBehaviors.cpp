#include "WifeBehaviors.h"
#include "MinersWife.h"
#include "Locations.h"
#include "EntityNames.h"
#include "misc/Utils.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

namespace
{
  void PrintWifeLine(MinersWife* wife, const char* message)
  {
    cout << "\n" << GetNameOfEntity(wife->ID()) << ": " << message;
  }

  void PrintExitActivity(MinersWife* wife)
  {
    switch (wife->CurrentActivity())
    {
    case wife_activity_bathroom:
      PrintWifeLine(wife, "Leavin' the Jon");
      break;

    default:
      break;
    }
  }

  void EnterActivity(MinersWife* wife, wife_activity newActivity)
  {
    if (wife->CurrentActivity() == newActivity)
    {
      return;
    }

    PrintExitActivity(wife);
    wife->ChangeActivity(newActivity);

    if (newActivity == wife_activity_bathroom)
    {
      PrintWifeLine(wife, "Walkin' to the can. Need to powda mah pretty li'lle nose");
    }
  }

  bool NeedsBathroom(MinersWife*)
  {
    //The original FSM global state checked this before house work.
    return RandFloat() < 0.1;
  }

  class DoHouseWorkAction : public BehaviorNode<MinersWife>
  {
  public:

    virtual BehaviorStatus Tick(MinersWife* wife)
    {
      EnterActivity(wife, wife_activity_housework);

      switch(RandInt(0,2))
      {
      case 0:
        PrintWifeLine(wife, "Moppin' the floor");
        break;

      case 1:
        PrintWifeLine(wife, "Washin' the dishes");
        break;

      case 2:
        PrintWifeLine(wife, "Makin' the bed");
        break;
      }

      return success;
    }
  };

  class VisitBathroomAction : public BehaviorNode<MinersWife>
  {
  public:

    virtual BehaviorStatus Tick(MinersWife* wife)
    {
      EnterActivity(wife, wife_activity_bathroom);
      PrintWifeLine(wife, "Ahhhhhh! Sweet relief!");
      PrintWifeLine(wife, "Leavin' the Jon");
      wife->ChangeActivity(wife_activity_none);

      return success;
    }
  };

  SequenceNode<MinersWife>* MakeSequence(BehaviorNode<MinersWife>* condition, BehaviorNode<MinersWife>* action)
  {
    SequenceNode<MinersWife>* sequence = new SequenceNode<MinersWife>;
    sequence->AddChild(condition);
    sequence->AddChild(action);

    return sequence;
  }
}

BehaviorNode<MinersWife>* CreateWifeBehaviorTree()
{
  SelectorNode<MinersWife>* root = new SelectorNode<MinersWife>;

  root->AddChild(MakeSequence(new ConditionNode<MinersWife>(NeedsBathroom), new VisitBathroomAction));
  root->AddChild(new DoHouseWorkAction);

  return root;
}
