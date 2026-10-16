#ifndef BANKHAR_H
#define BANKHAR_H

#include "BaseGameEntity.h"
#include "BehaviorTree.h"

const int    HungerLevel = 6;    //배고픔이 이 값 이상이면 밥을 조른다
const double WolfChance  = 0.1;  //매 턴 늑대 냄새를 맡을 확률 (늑대는 랜덤 사건)

class Bankhar : public BaseGameEntity
{
private:

  BehaviorNode<Bankhar>*  m_pBehaviorTree;

  int                     m_iHunger;       //배고픔

  bool                    m_bFoodArrived;  //메모 : "밥 먹어라"를 받았다

  //메모 : "낑낑"을 이미 보냈다.
  //BT에는 "상태에 들어갈 때 한 번"이 없고 매 턴 같은 행동이 다시 Tick 되므로,
  //이 메모가 없으면 밥을 기다리는 동안 매 턴 "낑낑"을 보내게 된다.
  bool                    m_bBegged;

public:

  Bankhar(int id);

  ~Bankhar();

  void Update();

  bool HandleMessage(const Telegram& msg);

  int  Hunger()const{return m_iHunger;}
  void IncreaseHunger(){m_iHunger += 1;}
  void ResetHunger(){m_iHunger = 0;}
  bool Hungry()const{return m_iHunger >= HungerLevel;}

  bool FoodArrived()const{return m_bFoodArrived;}
  void ClearFoodArrived(){m_bFoodArrived = false;}

  bool Begged()const{return m_bBegged;}
  void SetBegged(bool val){m_bBegged = val;}
};

#endif
