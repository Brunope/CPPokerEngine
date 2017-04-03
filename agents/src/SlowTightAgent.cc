#include "SlowTightAgent.h"

Action
SlowTightAgent::act(const GameView &view) {
  Sleeper::sleep();
  return TightAgent::act(view);
}
