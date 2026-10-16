#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>

class BaseGameEntity;

#define EntityMgr EntityManager::Instance()

class EntityManager
{
private:

  std::map<int, BaseGameEntity*> m_EntityMap;

public:

  static EntityManager* Instance();

  void            RegisterEntity(BaseGameEntity* NewEntity);

  BaseGameEntity* GetEntityFromID(int id)const;
};

#endif
