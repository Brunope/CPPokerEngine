#ifndef SIMPLEACTOR_H_
#define SIMPLEACTOR_H_

#include <utility>

#include "GameView.h"
#include "HandHistory.h"
#include "Actor.h"

class SimpleActor : public Actor {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &gameView);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // SIMPLEACTOR_H_
