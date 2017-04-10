#include <chrono>
#include <cassert>
#include "Random.h"

std::mt19937 Random::rng_;
unsigned int Random::seed_;

void
Random::seed() {
#ifdef __MINGW32__  // f u windows
  seed_ = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
  rng_.seed(seed_);
#else
  std::random_device rd;
  seed_ = rd();
  rng_.seed(seed_);
#endif
}

void
Random::setSeed(unsigned int seed) {
  seed_ = seed;
  rng_.seed(seed);
}

unsigned int
Random::getSeed() {
  return seed_;
}

int
Random::getInRange(int lo, int hi) {
  assert(hi - lo > 0);
  return rng_() % (hi + 1 - lo) + lo;
}
