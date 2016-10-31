#ifndef CALL_H_
#define CALL_H_

#include <cinttypes>

#include "Action.h"

/*
 * Forgive the redundance
 */
class Call : public Action {
public:
  Call(size_t amount) : Action(nullptr), amount_(amount) { }
  Call(size_t amount, Player* source) : Action(source), amount_(amount) {}

  size_t getValue() { return amount_; }
  inline ACTION_T getType() const { return CALL; }
private:
  size_t amount_;
};
#endif  // CALL_H_
