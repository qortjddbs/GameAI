#include "MinersWife.h"
#include "WifeBehaviors.h"
#include "misc/ConsoleUtils.h"

MinersWife::MinersWife(int id):BaseGameEntity(id),
                               m_Location(shack),
                               m_CurrentActivity(wife_activity_none),
                               m_pBehaviorTree(CreateWifeBehaviorTree())
{}

MinersWife::~MinersWife()
{
  delete m_pBehaviorTree;
}

void MinersWife::Update()
{
  //set text color to green
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  if (m_pBehaviorTree)
  {
    m_pBehaviorTree->Tick(this);
  }
}
