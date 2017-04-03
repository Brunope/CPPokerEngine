#include "SlowRandomAgent.h"

Action
SlowRandomAgent::act(const GameView &view) {
  Sleeper::sleep();
  return RandomAgent::act(view);
}
