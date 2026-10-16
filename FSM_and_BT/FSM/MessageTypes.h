#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES
//------------------------------------------------------------------------
//  게임 인공지능 과제 : FSM With Messaging vs Behavior Tree (몽골 유목민 가족)
//  학번 : 2022180016    성명 : 백성윤
//  MessageTypes.h : 메시지 5종
//------------------------------------------------------------------------

enum message_type
{
  Msg_WolfSpotted,   //반하르 -> 바트   : 늑대다!
  Msg_YaksHome,      //바트   -> 체첵   : 야크 데려왔어
  Msg_DogHungry,     //반하르 -> 체첵   : 낑낑 (배고파요)
  Msg_FoodReady,     //체첵   -> 반하르 : 밥 먹어라
  Msg_AaruulDry      //체첵   -> 체첵   : 아롤 다 말랐다 (예약 메시지)
};

inline const char* MsgToStr(int msg)
{
  static const char* const names[] = { "늑대다!", "야크 데려왔어", "낑낑 (배고파요)", "밥 먹어라", "아롤 다 말랐다" };

  return names[msg];
}

#endif
