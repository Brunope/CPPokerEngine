#include <cinttypes>
#include <cassert>

#include "Debug.h"
#include "Action.h"

Action::Action(ACTION_T type, uint32_t amount, Player *source) {
  type_ = type;
  amount_ = amount;
  source_ = source;
}

ACTION_T
Action::getType() const {
  return type_;
}

const Player *
Action::getSource() const {
  return source_;
}

uint32_t
Action::getAmount() const {
  return amount_;
}

void
Action::checkRep() {
  assert(type_ < NUM_ACTIONS);
}
