#ifndef RANDOMAGENT_H_
#define RANDOMAGENT_H_

#include "Random.h"
#include "Player.h"
#include "HandHistory.h"
#include "Card.h"
#include "GameView.h"
#include "Agent.h"

/**
 * Always returns a random legal Action from act().
 */
class RandomAgent : public Agent {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);
};

#endif  // RANDOMAGENT_H_
