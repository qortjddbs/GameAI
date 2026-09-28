#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H
//------------------------------------------------------------------------
//
//  Name:   BehaviorTree.h
//
//  Desc:   A tiny behavior tree implementation used by Miner Bob.
//
//------------------------------------------------------------------------
#include <vector>

class Miner;

enum BehaviorStatus
{
  failure,
  success,
  running
};

class BehaviorNode
{
public:

  virtual ~BehaviorNode(){}

  virtual BehaviorStatus Tick(Miner* miner) = 0;
};

class CompositeNode : public BehaviorNode
{
protected:

  typedef std::vector<BehaviorNode*> Children;

  Children m_Children;

public:

  virtual ~CompositeNode()
  {
    for (Children::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
    {
      delete *it;
    }
  }

  void AddChild(BehaviorNode* child)
  {
    m_Children.push_back(child);
  }
};

class SequenceNode : public CompositeNode
{
public:

  virtual BehaviorStatus Tick(Miner* miner)
  {
    for (Children::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
    {
      BehaviorStatus status = (*it)->Tick(miner);

      if (status != success)
      {
        return status;
      }
    }

    return success;
  }
};

class SelectorNode : public CompositeNode
{
public:

  virtual BehaviorStatus Tick(Miner* miner)
  {
    for (Children::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
    {
      BehaviorStatus status = (*it)->Tick(miner);

      if (status != failure)
      {
        return status;
      }
    }

    return failure;
  }
};

class ConditionNode : public BehaviorNode
{
private:

  bool (*m_pCondition)(Miner*);

public:

  ConditionNode(bool (*condition)(Miner*)) : m_pCondition(condition) {}

  virtual BehaviorStatus Tick(Miner* miner)
  {
    return m_pCondition(miner) ? success : failure;
  }
};

#endif
