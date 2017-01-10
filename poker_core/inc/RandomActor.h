#ifndef RANDOMACTOR_H_
#define RANDOMACTOR_H_

#include <random>

#include "Player.h"
#include "HandHistory.h"
#include "Card.h"
#include "GameView.h"
#include "Actor.h"

/**
 * Always returns a random legal Action from act().
 */
class RandomActor : public Actor {
public:
  RandomActor();
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);
private:
  std::mt19937 mt_;
  std::uniform_int_distribution<int> dist_;
};

#endif  // RANDOMACTOR_H_
