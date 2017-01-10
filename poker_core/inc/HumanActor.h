#ifndef HUMANACTOR_H_
#define HUMANACTOR_H_

#include "Action.h"
#include "Player.h"
#include "HandHistory.h"
#include "Card.h"
#include "GameView.h"
#include "Actor.h"

class HumanActor : public Actor {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // HUMANACTOR_H_
