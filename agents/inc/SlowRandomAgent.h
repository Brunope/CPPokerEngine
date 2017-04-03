#ifndef SLOWRANDOMAGENT_H_
#define SLOWRANDOMAGENT_H_

#include "RandomAgent.h"
#include "Sleeper.h"

class SlowRandomAgent : public RandomAgent, public Sleeper {
public:
  Action act(const GameView &view);
};

#endif  // SLOWRANDOMAGENT_H_
