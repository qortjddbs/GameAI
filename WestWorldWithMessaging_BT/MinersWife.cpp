#include "MinersWife.h"
#include "WifeBehaviors.h"
#include "MessageTypes.h"
#include "MessageDispatcher.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"
#include "messaging/Telegram.h"
#include "misc/ConsoleUtils.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

MinersWife::MinersWife(int id):m_pBehaviorTree(CreateWifeBehaviorTree()),
                              m_CurrentActivity(wife_activity_none),
                              m_Location(shack),
                              m_bCooking(false),
                              m_bHoneyHome(false),
                              BaseGameEntity(id)
{
}

MinersWife::~MinersWife()
{
  delete m_pBehaviorTree;
}

bool MinersWife::HandleMessage(const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
  case Msg_HiHoneyImHome:

    cout << "\nMessage handled by " << GetNameOfEntity(ID()) << " at time: "
         << Clock->GetCurrentTime();

    SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(ID())
         << ": Hi honey. Let me make you some of mah fine country stew";

    m_bHoneyHome = true;

    return true;

  case Msg_StewReady:

    cout << "\nMessage received by " << GetNameOfEntity(ID())
         << " at time: " << Clock->GetCurrentTime();

    SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(ID()) << ": StewReady! Lets eat";

    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
                              ID(),
                              ent_Miner_Bob,
                              Msg_StewReady,
                              NO_ADDITIONAL_INFO);

    SetCooking(false);
    SetHoneyIsHome(false);

    cout << "\n" << GetNameOfEntity(ID()) << ": Puttin' the stew on the table";

    ChangeActivity(wife_activity_none);

    return true;
  }

  return false;
}

void MinersWife::Update()
{
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  m_pBehaviorTree->Tick(this);
}
