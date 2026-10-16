#ifndef NAMES_H
#define NAMES_H
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  EntityNames.h : 등장인물 ID와 이름
//------------------------------------------------------------------------

enum
{
  ent_Father_Bat,      //아버지 바트 (목동)
  ent_Mother_Tsetseg,  //어머니 체첵
  ent_Bankhar          //경비견 반하르
};

inline const char* GetNameOfEntity(int n)
{
  static const char* const names[] = { "바트", "체첵", "반하르" };

  return names[n];
}

#endif
