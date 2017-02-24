#include <thread>
#include <chrono>
#include "SlowAgent.h"

SlowAgent::SlowAgent() {
  wait_millis_ = 500;
}

void
SlowAgent::setWaitTime(uint32_t millis) {
  wait_millis_ = millis;
}

uint32_t
SlowAgent::getWaitTime() {
  return wait_millis_;
}

Action
SlowAgent::act(const GameView &view) {
  std::this_thread::sleep_for(std::chrono::milliseconds(wait_millis_));
  return RandomAgent::act(view);
}
