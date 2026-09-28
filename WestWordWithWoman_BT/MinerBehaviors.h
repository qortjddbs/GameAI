#ifndef MINER_BEHAVIORS_H
#define MINER_BEHAVIORS_H

#include "BehaviorTree.h"

class Miner;

BehaviorNode<Miner>* CreateMinerBehaviorTree();

#endif
