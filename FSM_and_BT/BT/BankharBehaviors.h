#ifndef BANKHAR_BEHAVIORS_H
#define BANKHAR_BEHAVIORS_H
//------------------------------------------------------------------------
//    Selector (위가 우선)
//     1. [늑대 냄새? (매 턴 10%)]  -> 짖어서 경고 ("늑대다!")   <- FSM 전역 상태가 옮겨 온 가지
//     2. [밥 도착 메모?]           -> 밥 먹기
//     3. [배고픔 6 이상?]          -> 밥 조르기 (처음 한 번만 "낑낑", 그 뒤로는 기다림)
//     4. 게르 지키기
//------------------------------------------------------------------------
#include "BehaviorTree.h"

class Bankhar;

BehaviorNode<Bankhar>* CreateBankharBehaviorTree();

#endif
