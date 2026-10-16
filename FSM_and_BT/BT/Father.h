#ifndef FATHER_H
#define FATHER_H

#include "BaseGameEntity.h"
#include "BehaviorTree.h"

enum location_type
{
  ger,      //게르 (집)
  pasture   //초원
};

//야크 배부름이 이 값 이상이면 게르로 데려온다
const int YakFullLevel = 10;

class Father : public BaseGameEntity
{
private:

  BehaviorNode<Father>*  m_pBehaviorTree;

  //FSM의 "지금 방목 중"이라는 상태 기억 대신, 위치(게르/초원)로 판단한다
  location_type          m_Location;

  int                    m_iFatigue;      //피로도
  int                    m_iYakFullness;  //야크 배부름

  bool                   m_bWolfAlert;    //메모 : "늑대다!"를 받았다

public:

  Father(int id);

  ~Father();

  void Update();

  bool HandleMessage(const Telegram& msg);

  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location = loc;}

  int           Fatigue()const{return m_iFatigue;}
  void          IncreaseFatigue(int val){m_iFatigue += val;}
  void          DecreaseFatigue(){m_iFatigue -= 1;}
  bool          Tired()const{return m_iFatigue > 0;}

  int           YakFullness()const{return m_iYakFullness;}
  void          FeedYaks(){m_iYakFullness += 2;}
  bool          YaksFull()const{return m_iYakFullness >= YakFullLevel;}
  void          ResetYakFullness(){m_iYakFullness = 0;}

  bool          WolfAlert()const{return m_bWolfAlert;}
  void          ClearWolfAlert(){m_bWolfAlert = false;}
};

#endif
