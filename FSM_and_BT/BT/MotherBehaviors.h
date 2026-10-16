#ifndef MOTHER_BEHAVIORS_H
#define MOTHER_BEHAVIORS_H
//------------------------------------------------------------------------
//    Selector (위가 우선)
//     1. [반하르 배고픔 메모?]  -> 반하르 밥 준비하기 (2턴, 끝나면 "밥 먹어라")
//     2. [우유 3 이상?]         -> 아롤 만들기 (자신에게 "아롤 다 말랐다" 예약)
//     3. [야크 귀가 메모?]      -> 젖 짜기
//     4. 게르 살림
//------------------------------------------------------------------------
#include "BehaviorTree.h"

class Mother;

BehaviorNode<Mother>* CreateMotherBehaviorTree();

#endif
