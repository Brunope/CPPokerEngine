#ifndef FOLD_H_
#define FOLD_H_

#include "actions/Action.h"

class Fold : public Action {
public:
  Fold() : Action(nullptr) {}
  Fold(Player *source) : Action(source) {}

  inline ACTION_T getType() const { return FOLD; }
};
#endif  // FOLD_H_
