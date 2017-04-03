#include <thread>
#include <chrono>
#include "Sleeper.h"

Sleeper::Sleeper(uint32_t millis) {
  sleep_millis_ = millis;
}

void
Sleeper::setSleepTime(uint32_t millis) {
  sleep_millis_ = millis;
}

uint32_t
Sleeper::getSleepTime() {
  return sleep_millis_;
}

void
Sleeper::sleep() {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_millis_));
}
