#ifndef FATHER_H
#define FATHER_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Father.h [FSM] : 아버지 바트 (목동). 야크를 초원에 데려가 풀을 먹이고 지킨다.
//------------------------------------------------------------------------
#include "BaseGameEntity.h"
#include "StateMachine.h"

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

  StateMachine<Father>  m_StateMachine;

  location_type         m_Location;

  int                   m_iFatigue;      //피로도
  int                   m_iYakFullness;  //야크 배부름

public:

  Father(int id);

  void Update();

  bool HandleMessage(const Telegram& msg);

  StateMachine<Father>* GetFSM(){return &m_StateMachine;}

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
};

#endif
