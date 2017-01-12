#include <cinttypes>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

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

std::string
Action::str() const {
  std::stringstream ss;
  ss << source_->getName() << " ";
  switch (type_) {
  case RAISE:
    ss << "raises " << amount_;
    break;
  case CALL:
    ss << "calls " << amount_;
    break;
  case FOLD:
    ss << "folds";
    break;
  case CHECK:
    ss << "checks";
    break;
  case POST:
    ss << "posts " << amount_;
    break;
  default:
    assert(type_ < NUM_ACTIONS);
    break;
  }

  return ss.str();
}

std::ostream &
operator<<(std::ostream &os, const Action &action) {
  os << action.str();
  return os;
}
