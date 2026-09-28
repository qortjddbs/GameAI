#ifndef MINERSWIFE_H
#define MINERSWIFE_H

#include <string>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "BehaviorTree.h"

struct Telegram;
class MinersWife;

enum wife_activity
{
  wife_activity_none,
  wife_activity_housework,
  wife_activity_bathroom,
  wife_activity_cooking
};

class MinersWife : public BaseGameEntity
{
private:

  BehaviorNode<MinersWife>*  m_pBehaviorTree;

  wife_activity              m_CurrentActivity;

  location_type              m_Location;

  bool                       m_bCooking;
  bool                       m_bHoneyHome;

public:

  MinersWife(int id);

  ~MinersWife();

  void Update();

  virtual bool HandleMessage(const Telegram& msg);

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(const location_type loc){m_Location=loc;}

  wife_activity CurrentActivity()const{return m_CurrentActivity;}
  void          ChangeActivity(const wife_activity activity){m_CurrentActivity = activity;}

  bool          Cooking()const{return m_bCooking;}
  void          SetCooking(const bool val){m_bCooking = val;}

  bool          HoneyIsHome()const{return m_bHoneyHome;}
  void          SetHoneyIsHome(const bool val){m_bHoneyHome = val;}
};

#endif
