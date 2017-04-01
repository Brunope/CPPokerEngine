#ifndef HUMANAGENT_H_
#define HUMANAGENT_H_

#include "Action.h"
#include "Player.h"
#include "HandHistory.h"
#include "Card.h"
#include "GameView.h"
#include "Agent.h"

class HumanAgent : public Agent {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // HUMANAGENT_H_
