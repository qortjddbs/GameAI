#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include <vector>

//running : 행동이 아직 끝나지 않아 다음 턴에도 이어진다 (FSM에서 한 상태에 여러 턴 머무는 것에 해당)
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

  std::vector<BehaviorNode<entity_type>*> m_Children;

public:

  ~CompositeNode()
  {
    for (BehaviorNode<entity_type>* child : m_Children)
    {
      delete child;
    }
  }

  void AddChild(BehaviorNode<entity_type>* child)
  {
    m_Children.push_back(child);
  }
};

//Sequence : 자식을 차례로 실행하고, success가 아닌 결과가 나오면 거기서 멈춘다 ("조건 -> 행동")
template <class entity_type>
class SequenceNode : public CompositeNode<entity_type>
{
public:

  BehaviorStatus Tick(entity_type* entity) override
  {
    for (BehaviorNode<entity_type>* child : this->m_Children)
    {
      BehaviorStatus status = child->Tick(entity);

      if (status != success)
      {
        return status;
      }
    }

    return success;
  }
};

//Selector : 위(먼저 추가한 자식)부터 시도해서 failure가 아닌 첫 자식을 고른다 (우선순위)
template <class entity_type>
class SelectorNode : public CompositeNode<entity_type>
{
public:

  BehaviorStatus Tick(entity_type* entity) override
  {
    for (BehaviorNode<entity_type>* child : this->m_Children)
    {
      BehaviorStatus status = child->Tick(entity);

      if (status != failure)
      {
        return status;
      }
    }

    return failure;
  }
};

//Condition : 질문(bool 함수)을 success / failure 로 바꿔 준다
template <class entity_type>
class ConditionNode : public BehaviorNode<entity_type>
{
private:

  bool (*m_pCondition)(entity_type*);

public:

  ConditionNode(bool (*condition)(entity_type*)) : m_pCondition(condition) {}

  BehaviorStatus Tick(entity_type* entity) override
  {
    return m_pCondition(entity) ? success : failure;
  }
};

//"조건 -> 행동" 한 쌍을 Sequence 로 묶는다
template <class entity_type>
BehaviorNode<entity_type>* MakeSequence(bool (*condition)(entity_type*), BehaviorNode<entity_type>* action)
{
  SequenceNode<entity_type>* sequence = new SequenceNode<entity_type>;

  sequence->AddChild(new ConditionNode<entity_type>(condition));
  sequence->AddChild(action);

  return sequence;
}

#endif
