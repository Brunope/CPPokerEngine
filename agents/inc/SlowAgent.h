#ifndef SLOWAGENT_H_
#define SLOWAGENT_H_

#include "RandomAgent.h"

/**
 * Waits for some amount of time before returning a random action
 */
class SlowAgent : public RandomAgent {
public:
  SlowAgent();
  
  void setWaitTime(uint32_t millis);
  uint32_t getWaitTime();

  Action act(const GameView &view);
private:
  uint32_t wait_millis_;
};

#endif  // SLOWAGENT_H_
