#include <string>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>
#include "Action.h"
#include "QHumanAgent.h"

QHumanAgent::QHumanAgent() {
  QObject(0);
  got_action_.store(false);
  seat_ = 0;
}

void
QHumanAgent::receiveHoleCards(const std::pair<Card, Card> hc) {
  hc0_.copyFrom(hc.first);
  hc1_.copyFrom(hc.second);
  emit holeCardsChanged();
  
  // weird place to do this i know
  int new_seat = static_cast<int>(self_->getSeat());
  if (seat_ != new_seat) {
    seat_ = new_seat;
    emit seatChanged();
  }
}

Action
QHumanAgent::act(const GameView &view) {
  UNUSED(view);
  emit needAction();

  // wait til ui sends the action to doAction, which will set got_action_,
  // to_do_action_type_ and to_do_action_amount_
  while (!got_action_.load()) {
    // todo: use a condition variable instead to go easier on the cpu
    std::this_thread::yield();
  }

  // don't think this is strictly safe, but doAction doesn't modify after
  // setting got_action_ so in this case it's ok..
  ACTION_T type_copy = static_cast<ACTION_T>(to_do_action_type_.load());
  uint32_t amount_copy = to_do_action_amount_.load();
  got_action_.store(false);
  return Action(type_copy, amount_copy);
}

void
QHumanAgent::receiveHandHistory(const HandHistory &history) {
  UNUSED(history);
}

void
QHumanAgent::doAction(int action_type, int amount) {
  assert(action_type >= 0 && action_type < NUM_ACTIONS);
  to_do_action_type_.store(static_cast<uint8_t>(action_type));
  to_do_action_amount_.store(static_cast<uint32_t>(amount));
  got_action_.store(true);
}

// property accessors

int
QHumanAgent::seat() const {
  return seat_;
}

QCard *
QHumanAgent::card0() {
  return &hc0_;
}

QCard *
QHumanAgent::card1() {
  return &hc1_;
}

