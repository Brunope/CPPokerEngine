#ifndef TIGHTAGENT_H_
#define TIGHTAGENT_H_

#include "GameView.h"
#include "HandHistory.h"
#include "Agent.h"

class TightAgent : public Agent {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // TIGHTAGENT_H_
