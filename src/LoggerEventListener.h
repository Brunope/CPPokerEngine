#ifndef LOGGER_EVENT_LISTENER_H_
#define LOGGER_EVENT_LISTENER_H_

#include "IEventListener.h"

class LoggerEventListener : public IEventListener {
public:
  LoggerEventListener();
  void onGameStart(const GameView *game);
  void onPlayerJoin(std::string playerName);
  void onPlayerLeave(std::string playerName);
  void onHandStart(long handNum, const GameView *game);
  void onDeal(STREET street);
  void onPlayerAction(Action action);
  void onShowdown(Hand bestHand, std::string playerName);
  void onPotWin(int pot, std::string playerName);
private:
  const GameView *game_;
};

#endif  // LOGGER_EVENT_LISTENER_H_
