#ifndef TESTAGENT_H_
#define TESTAGENT_H_

#include <utility>
#include <vector>

#include "GameView.h"
#include "Action.h"
#include "HandHistory.h"
#include "Agent.h"

// FIFO queue Actions for testing purposes. The first action queue'd will
// be returned on the first act() call.
class TestAgent : public Agent {
public:
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &gameView);
  void receiveHandHistory(const HandHistory &history);

  void queueAction(Action action);
private:
  std::vector<Action> action_queue_;
};

#endif  // TESTAGENT_H_
