#ifndef CHECK_H_
#define CHECK_H_

#include "actions/Action.h"

class Check : public Action {
public:
  Check(Player *source) : Action(source) {}

  inline ACTION_T getType() const { return CHECK; }
};
#endif  // CHECK_H_
