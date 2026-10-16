#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  BaseGameEntity.h : 모든 등장인물의 부모 클래스 (Buckland 원본 기반)
//------------------------------------------------------------------------
#include "Telegram.h"

class BaseGameEntity
{
private:

  int m_ID;

public:

  BaseGameEntity(int id):m_ID(id){}

  virtual ~BaseGameEntity(){}

  //매 턴 할 일
  virtual void Update()=0;

  //메시지를 받았을 때 할 일
  virtual bool HandleMessage(const Telegram& msg)=0;

  int ID()const{return m_ID;}
};

#endif
