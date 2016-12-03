#ifndef TESTACTOR_H_
#define TESTACTOR_H_

#include <utility>
#include <vector>

#include "GameView.h"
#include "Action.h"
#include "Actor.h"

// FIFO queue Actions for testing purposes. The first action queue'd will
// be returned on the first act() call.
class TestActor : public Actor {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &gameView);
  void receiveHandHistory(const std::vector<const Action> &history);

  void queueAction(Action action);
private:
  std::vector<Action> action_queue_;
};

#endif  // TESTACTOR_H_
