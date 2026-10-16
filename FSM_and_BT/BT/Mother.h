#ifndef MOTHER_H
#define MOTHER_H

#include "BaseGameEntity.h"
#include "BehaviorTree.h"

const int    MilkForAaruul  = 3;    //우유가 이만큼 모이면 아롤을 만든다
const int    FoodPrepTurns  = 2;    //반하르 밥 준비에 걸리는 턴 수
const double AaruulDryDelay = 3.6;  //아롤이 마르는 시간(초) = 예약 메시지 지연 시간 (4.5턴 -> 항상 5턴째 도착)

class Mother : public BaseGameEntity
{
private:

  BehaviorNode<Mother>*  m_pBehaviorTree;

  int                    m_iMilk;           //짠 우유
  int                    m_iAaruul;         //말려서 걷어 둔 아롤 개수
  int                    m_iFoodPrepCount;  //반하르 밥을 몇 턴째 준비 중인지

  bool                   m_bYaksHome;       //메모 : "야크 데려왔어"를 받았다
  bool                   m_bDogHungry;      //메모 : "낑낑"을 받았다

public:

  Mother(int id);

  ~Mother();

  void Update();

  bool HandleMessage(const Telegram& msg);

  int  Milk()const{return m_iMilk;}
  void AddMilk(){m_iMilk += 1;}
  void ResetMilk(){m_iMilk = 0;}
  bool EnoughMilk()const{return m_iMilk >= MilkForAaruul;}

  int  FoodPrepCount()const{return m_iFoodPrepCount;}
  void StartFoodPrep(){m_iFoodPrepCount = 0;}
  void PrepFood(){m_iFoodPrepCount += 1;}
  bool FoodPrepDone()const{return m_iFoodPrepCount >= FoodPrepTurns;}

  bool YaksHome()const{return m_bYaksHome;}
  void ClearYaksHome(){m_bYaksHome = false;}

  bool DogHungry()const{return m_bDogHungry;}
  void ClearDogHungry(){m_bDogHungry = false;}
};

#endif
