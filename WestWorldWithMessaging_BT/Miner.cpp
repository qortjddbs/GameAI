#include "Miner.h"
#include "MinerBehaviors.h"
#include "MessageTypes.h"
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

Miner::Miner(int id):m_pBehaviorTree(CreateMinerBehaviorTree()),
                     m_CurrentActivity(activity_none),
                     m_Location(shack),
                     m_iGoldCarried(0),
                     m_iMoneyInBank(0),
                     m_iThirst(0),
                     m_iFatigue(0),
                     m_bStewReady(false),
                     BaseGameEntity(id)
{
}

Miner::~Miner()
{
  delete m_pBehaviorTree;
}

bool Miner::HandleMessage(const Telegram& msg)
{
  SetTextColor(BACKGROUND_GREEN |BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
  case Msg_StewReady:

    cout << "\nMessage handled by " << GetNameOfEntity(ID())
         << " at time: " << Clock->GetCurrentTime();

    SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(ID()) << ": Okay Hun, ahm a comin'!";

    m_bStewReady = true;

    return true;
  }

  return false;
}

void Miner::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  m_iThirst += 1;
  
  m_pBehaviorTree->Tick(this);
}

void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}
