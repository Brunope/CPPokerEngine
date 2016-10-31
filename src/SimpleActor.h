#ifndef SIMPLEACTOR_H_
#define SIMPLEACTOR_H_

#include <utility>

#include "GameView.h"
#include "Actor.h"

class SimpleActor : public Actor {
public:
  SimpleActor(Player &self) : Actor(self) {}

  void receiveHoleCards(const std::pair<Card, Card> hc);
  
  Action act(const GameView &gameView);

  void receiveHandHistory(const std::vector<const Action> &history);
};

#endif  // SIMPLEACTOR_H_
