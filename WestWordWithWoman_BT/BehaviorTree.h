#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H
//------------------------------------------------------------------------
//
//  Name:   BehaviorTree.h
//
//  Desc:   A tiny templated behavior tree implementation.
//
//------------------------------------------------------------------------
#include <vector>

enum BehaviorStatus
{
  failure,
  success,
  running
};

template <class entity_type>
class BehaviorNode
{
public:

  virtual ~BehaviorNode(){}

  virtual BehaviorStatus Tick(entity_type* entity) = 0;
};

template <class entity_type>
class CompositeNode : public BehaviorNode<entity_type>
{
protected:

  typedef std::vector<BehaviorNode<entity_type>*> Children;

  Children m_Children;

public:

  virtual ~CompositeNode()
  {
    for (typename Children::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
    {
      delete *it;
    }
  }

  void AddChild(BehaviorNode<entity_type>* child)
  {
    m_Children.push_back(child);
  }
};

template <class entity_type>
class SequenceNode : public CompositeNode<entity_type>
{
private:

  typedef typename CompositeNode<entity_type>::Children Children;

public:

  virtual BehaviorStatus Tick(entity_type* entity)
  {
    for (typename Children::iterator it = this->m_Children.begin(); it != this->m_Children.end(); ++it)
    {
      BehaviorStatus status = (*it)->Tick(entity);

      if (status != success)
      {
        return status;
      }
    }

    return success;
  }
};

template <class entity_type>
class SelectorNode : public CompositeNode<entity_type>
{
private:

  typedef typename CompositeNode<entity_type>::Children Children;

public:

  virtual BehaviorStatus Tick(entity_type* entity)
  {
    for (typename Children::iterator it = this->m_Children.begin(); it != this->m_Children.end(); ++it)
    {
      BehaviorStatus status = (*it)->Tick(entity);

      if (status != failure)
      {
        return status;
      }
    }

    return failure;
  }
};

template <class entity_type>
class ConditionNode : public BehaviorNode<entity_type>
{
private:

  bool (*m_pCondition)(entity_type*);

public:

  ConditionNode(bool (*condition)(entity_type*)) : m_pCondition(condition) {}

  virtual BehaviorStatus Tick(entity_type* entity)
  {
    return m_pCondition(entity) ? success : failure;
  }
};

#endif
