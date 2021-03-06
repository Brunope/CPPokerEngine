#ifndef LOGGEREVENTLISTENER_H_
#define LOGGEREVENTLISTENER_H_

#include "IEventListener.h"

class LoggerEventListener : public IEventListener {
public:
  //LoggerEventListener();

  // seems redundant to redefine these, can I not?
  void onViewChanged(std::shared_ptr<const GameView> view);
  void onGameStart(std::shared_ptr<const GameView> view);
  void onGameEnd();
  void onPlayerJoin(Player player);
  void onPlayerLeave(Player player);
  void onHandStart(long handNum, std::shared_ptr<const GameView> view);
  void onDeal(STREET street);
  void onPlayerAction(Action action);
  void onShowCards(std::pair<Card, Card> cards, Player player);
  void onShowdown(Hand best_hand, Player player);
  void onPotWin(uint32_t pot, Player player);
private:
  std::shared_ptr<const GameView> view_;
};

#endif  // LOGGEREVENTLISTENER_H_
