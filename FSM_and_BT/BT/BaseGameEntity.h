#ifndef ENTITY_H
#define ENTITY_H

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
