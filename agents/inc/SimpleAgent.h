#ifndef SIMPLEAGENT_H_
#define SIMPLEAGENT_H_

#include <utility>

#include "GameView.h"
#include "HandHistory.h"
#include "Agent.h"

class SimpleAgent : public Agent {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &gameView);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // SIMPLEAGENT_H_
