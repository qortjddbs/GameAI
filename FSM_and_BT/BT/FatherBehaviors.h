#ifndef FATHER_BEHAVIORS_H
#define FATHER_BEHAVIORS_H
//------------------------------------------------------------------------
//    Selector (위가 우선)
//     1. [늑대 경보 메모?]           -> 늑대 쫓기
//     2. [게르에 있고 피로도 > 0?]   -> 게르에서 쉬기
//     3. 방목하기 (배부름 10이 되면 귀가하며 "야크 데려왔어")
//------------------------------------------------------------------------
#include "BehaviorTree.h"

class Father;

BehaviorNode<Father>* CreateFatherBehaviorTree();

#endif
