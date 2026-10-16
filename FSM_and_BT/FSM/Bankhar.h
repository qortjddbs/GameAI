#ifndef BANKHAR_H
#define BANKHAR_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  Bankhar.h [FSM] : 경비견 반하르. 게르를 지키고, 배고프면 밥을 조르고,
//                    늑대 냄새를 맡으면 짖어서 바트에게 알린다.
//------------------------------------------------------------------------
#include "BaseGameEntity.h"
#include "StateMachine.h"

const int    HungerLevel = 6;    //배고픔이 이 값 이상이면 밥을 조른다
const double WolfChance  = 0.1;  //매 턴 늑대 냄새를 맡을 확률 (늑대는 랜덤 사건)

class Bankhar : public BaseGameEntity
{
private:

  StateMachine<Bankhar>  m_StateMachine;

  int                    m_iHunger;   //배고픔

  //"낑낑"을 이미 보냈다는 메모.
  //[구현 중 발견한 문제] 밥 조르기 도중 짖기(상태 블립)에 다녀오면 RevertToPreviousState()가
  //밥 조르기의 Enter()를 다시 불러 "낑낑"이 두 번 나갔고, 그 결과 체첵이 밥 준비만 끝없이 반복했다.
  //상태는 "그 안에서 무엇을 했는지" 기억하지 못하므로 캐릭터에 메모를 두어 한 번만 보내게 했다.
  bool                   m_bBegged;

public:

  Bankhar(int id);

  void Update();

  bool HandleMessage(const Telegram& msg);

  StateMachine<Bankhar>* GetFSM(){return &m_StateMachine;}

  int  Hunger()const{return m_iHunger;}
  void IncreaseHunger(){m_iHunger += 1;}
  void ResetHunger(){m_iHunger = 0;}
  bool Hungry()const{return m_iHunger >= HungerLevel;}

  bool Begged()const{return m_bBegged;}
  void SetBegged(bool val){m_bBegged = val;}
};

#endif
