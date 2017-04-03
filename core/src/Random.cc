#include "Random.h"

std::mt19937 Random::rng_;

void
Random::seed() {
  std::random_device rd;
  rng_.seed(rd());
}

void
Random::setSeed(int seed) {
  rng_.seed(seed);
}

int
Random::getInRange(int lo, int hi) {
  return std::uniform_int_distribution<int>(lo, hi)(rng_);
}
