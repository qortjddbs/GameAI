#ifndef STATEMACHINE_H
#define STATEMACHINE_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  StateMachine.h : 현재 상태 / 직전 상태 / 전역 상태를 관리하는 상태 기계 (Buckland 원본 기반)
//------------------------------------------------------------------------
#include "State.h"

template <class entity_type>
class StateMachine
{
private:

  entity_type*          m_pOwner;

  State<entity_type>*   m_pCurrentState;

  //상태 블립(잠깐 다녀오는 상태)이 끝나면 돌아갈 곳
  State<entity_type>*   m_pPreviousState;

  //지금 상태와 상관없이 매 턴 함께 실행되는 전역 상태
  State<entity_type>*   m_pGlobalState;

public:

  StateMachine(entity_type* owner, State<entity_type>* startState, State<entity_type>* globalState)
    : m_pOwner(owner), m_pCurrentState(startState), m_pPreviousState(nullptr), m_pGlobalState(globalState) {}

  //전역 상태 -> 현재 상태 순서로 실행
  void Update()const
  {
    m_pGlobalState->Execute(m_pOwner);

    m_pCurrentState->Execute(m_pOwner);
  }

  //현재 상태가 먼저 받고, 처리하지 못하면 전역 상태가 받는다
  bool HandleMessage(const Telegram& msg)const
  {
    return m_pCurrentState->OnMessage(m_pOwner, msg) || m_pGlobalState->OnMessage(m_pOwner, msg);
  }

  void ChangeState(State<entity_type>* pNewState)
  {
    m_pPreviousState = m_pCurrentState;

    m_pCurrentState->Exit(m_pOwner);

    m_pCurrentState = pNewState;

    m_pCurrentState->Enter(m_pOwner);
  }

  void RevertToPreviousState()
  {
    ChangeState(m_pPreviousState);
  }
};

#endif
