#ifndef MINERSWIFE_H
#define MINERSWIFE_H

#include <string>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "BehaviorTree.h"

class MinersWife;

enum wife_activity
{
  wife_activity_none,
  wife_activity_housework,
  wife_activity_bathroom
};

class MinersWife : public BaseGameEntity
{
private:

  BehaviorNode<MinersWife>*  m_pBehaviorTree;

  wife_activity              m_CurrentActivity;

  location_type              m_Location;

public:

  MinersWife(int id);

  ~MinersWife();

  void Update();

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(const location_type loc){m_Location=loc;}

  wife_activity CurrentActivity()const{return m_CurrentActivity;}
  void          ChangeActivity(const wife_activity activity){m_CurrentActivity = activity;}
};

#endif
