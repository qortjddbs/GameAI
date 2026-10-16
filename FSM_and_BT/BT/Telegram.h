#ifndef TELEGRAM_H
#define TELEGRAM_H

struct Telegram
{
  int    Sender;        //보내는 쪽 ID
  int    Receiver;      //받는 쪽 ID
  int    Msg;           //메시지 종류 (MessageTypes.h)
  double DispatchTime;  //예약 메시지가 배달될 시각(초)

  Telegram(double time, int sender, int receiver, int msg)
    : Sender(sender), Receiver(receiver), Msg(msg), DispatchTime(time) {}
};

//예약 메시지 우편함(std::set)을 배달 시각 순으로 정렬하기 위한 비교
inline bool operator<(const Telegram& t1, const Telegram& t2)
{
  return t1.DispatchTime < t2.DispatchTime;
}

#endif
