//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  EntityManager.cpp
//------------------------------------------------------------------------
#include "EntityManager.h"
#include "BaseGameEntity.h"

EntityManager* EntityManager::Instance()
{
  static EntityManager instance;

  return &instance;
}

void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
  m_EntityMap[NewEntity->ID()] = NewEntity;
}

BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
  return m_EntityMap.find(id)->second;
}
