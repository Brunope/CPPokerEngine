#ifndef RAISE_H_
#define RAISE_H_

#include <cstddef>

#include "Action.h"

class Raise : public Action {
public:
  Raise(size_t amount) : Action(nullptr), amount_(amount) { }
  Raise(Player *source, size_t amount) : Action(source), amount_(amount) { }

  size_t getValue() const { return amount_; }
  inline ACTION_T getType() const { return RAISE; }
private:
  size_t amount_;
};
#endif  // RAISE_H_
