#include "SlowAgent.h"

Action
SlowAgent::act(const GameView &view) {
  Sleeper::sleep();
  return RandomAgent::act(view);
}
