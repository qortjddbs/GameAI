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
