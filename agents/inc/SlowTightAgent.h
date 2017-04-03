#ifndef SLOWTIGHTAGENT_H_
#define SLOWTIGHTAGENT_H_

#include "TightAgent.h"
#include "Sleeper.h"

class SlowTightAgent : public TightAgent, public Sleeper {
public:
  Action act(const GameView &view);
};

#endif  // SLOWTIGHTAGENT_H_
