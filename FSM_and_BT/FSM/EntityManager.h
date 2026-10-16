#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  EntityManager.h : ID로 등장인물을 찾아 주는 주소록 (Buckland 원본 기반)
//------------------------------------------------------------------------
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
