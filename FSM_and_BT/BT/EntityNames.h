#ifndef NAMES_H
#define NAMES_H

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
