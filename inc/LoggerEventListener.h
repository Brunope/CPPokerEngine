#ifndef LOGGER_EVENT_LISTENER_H_
#define LOGGER_EVENT_LISTENER_H_

#include "IEventListener.h"

class LoggerEventListener : public IEventListener {
public:
  LoggerEventListener();

  // seems redundant to redefine these, can I not?
  void onGameStart(const GameView *view);
  void onPlayerJoin(std::string name);
  void onPlayerLeave(std::string name);
  void onHandStart(long handNum, const GameView *view);
  void onDeal(STREET street);
  void onPlayerAction(Action action);
  void onShowCards(std::pair<Card, Card> cards, std::string name);
  void onShowdown(Hand best_hand, std::string name);
  void onPotWin(int pot, std::string name);
private:
  const GameView *view_;
};

#endif  // LOGGER_EVENT_LISTENER_H_
