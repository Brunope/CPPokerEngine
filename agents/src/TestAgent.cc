#include <cassert>

#include "HandHistory.h"
#include "TestAgent.h"

void
TestAgent::receiveHoleCards(const std::pair<Card, Card> hc) {
  hc_ = hc;
}

Action
TestAgent::act(const GameView &view) {
  assert(action_queue_.size() > 0);
  // no pop_front :(
  Action action = action_queue_.front();
  action_queue_.erase(action_queue_.begin());
  return action;
}

void
TestAgent::receiveHandHistory(const HandHistory &history) {}

void
TestAgent::queueAction(Action action) {
  action_queue_.push_back(action);
}
