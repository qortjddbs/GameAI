//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  main.cpp [FSM] : 매 턴 바트 -> 체첵 -> 반하르 Update, 예약 메시지 배달, 0.8초 대기
//------------------------------------------------------------------------
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

#include "Father.h"
#include "Mother.h"
#include "Bankhar.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "EntityNames.h"

using std::cout;

const char* const STUDENT_ID   = "2022180016";
const char* const STUDENT_NAME = "백성윤";

//난수(rand)는 반하르의 늑대 냄새에만 쓰인다. FSM과 BT가 같은 시드를 쓰면 늑대가 같은 턴에 나타난다.
//208 : 40턴 안에 비교 장면이 모두 나오는 시드
//      (방목 중 늑대 14턴 / 밥 준비 중 야크 귀가 30턴 / 밥 조르기 중 늑대 35턴 / 방목 나서는 순간 늑대 35턴)
const unsigned int RandomSeed = 208;
const int          NumTurns   = 40;

static void PrintTitle()
{
  cout << "==========================================================";
  cout << "\n [FSM With Messaging] 몽골 초원 유목민 가족의 하루";
  cout << "\n 학번 : " << STUDENT_ID << "    성명 : " << STUDENT_NAME;
  cout << "\n==========================================================";
}

int main()
{
  srand(RandomSeed);

  cout << std::fixed << std::setprecision(1);   //시간(초)을 소수점 한 자리로 출력

  Father*  Bat     = new Father(ent_Father_Bat);
  Mother*  Tsetseg = new Mother(ent_Mother_Tsetseg);
  Bankhar* Dog     = new Bankhar(ent_Bankhar);

  EntityMgr->RegisterEntity(Bat);
  EntityMgr->RegisterEntity(Tsetseg);
  EntityMgr->RegisterEntity(Dog);

  PrintTitle();

  for (int turn = 1; turn <= NumTurns; ++turn)
  {
    cout << "\n\n------------------------- 턴 " << turn << " -------------------------";

    Bat->Update();
    Tsetseg->Update();
    Dog->Update();

    Dispatch->DispatchDelayedMessages();

    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }

  cout << "\n\n";
  PrintTitle();
  cout << "\n";

  delete Bat;
  delete Tsetseg;
  delete Dog;

  return 0;
}
