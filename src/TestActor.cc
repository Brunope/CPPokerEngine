#include <cassert>
#include "TestActor.h"

void
TestActor::receiveHoleCards(const std::pair<Card, Card> hc) {
  hc_ = hc;
}

Action
TestActor::act(const GameView &view) {
  assert(action_queue_.size() > 0);
  // no pop_front :(
  Action action = action_queue_.front();
  action_queue_.erase(action_queue_.begin());
  return action;
}

void
TestActor::receiveHandHistory(const std::vector<const Action> &history) {}

void
TestActor::queueAction(Action action) {
  action_queue_.push_back(action);
}
