#ifndef MOTHER_H
#define MOTHER_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Mother.h [FSM] : 어머니 체첵. 야크 젖을 짜서 아롤(말린 치즈)을 만들고, 반하르 밥을 준다.
//------------------------------------------------------------------------
#include "BaseGameEntity.h"
#include "StateMachine.h"

const int    MilkForAaruul  = 3;    //우유가 이만큼 모이면 아롤을 만든다
const int    FoodPrepTurns  = 2;    //반하르 밥 준비에 걸리는 턴 수
const double AaruulDryDelay = 3.6;  //아롤이 마르는 시간(초) = 예약 메시지 지연 시간 (4.5턴 -> 항상 5턴째 도착)

class Mother : public BaseGameEntity
{
private:

  StateMachine<Mother>  m_StateMachine;

  int                   m_iMilk;           //짠 우유
  int                   m_iAaruul;         //말려서 걷어 둔 아롤 개수
  int                   m_iFoodPrepCount;  //반하르 밥을 몇 턴째 준비 중인지

public:

  Mother(int id);

  void Update();

  bool HandleMessage(const Telegram& msg);

  StateMachine<Mother>* GetFSM(){return &m_StateMachine;}

  int  Milk()const{return m_iMilk;}
  void AddMilk(){m_iMilk += 1;}
  void ResetMilk(){m_iMilk = 0;}
  bool EnoughMilk()const{return m_iMilk >= MilkForAaruul;}

  int  Aaruul()const{return m_iAaruul;}
  void AddAaruul(){m_iAaruul += 1;}

  int  FoodPrepCount()const{return m_iFoodPrepCount;}
  void StartFoodPrep(){m_iFoodPrepCount = 0;}
  void PrepFood(){m_iFoodPrepCount += 1;}
  bool FoodPrepDone()const{return m_iFoodPrepCount >= FoodPrepTurns;}
};

#endif
