#include "Random.h"

std::mt19937 Random::rng_;
uint Random::seed_;

void
Random::seed() {
  std::random_device rd;
  seed_ = rd();
  rng_.seed(seed_);
}

void
Random::setSeed(uint seed) {
  seed_ = seed;
  rng_.seed(seed);
}

uint
Random::getSeed() {
  return seed_;
}

int
Random::getInRange(int lo, int hi) {
  return std::uniform_int_distribution<int>(lo, hi)(rng_);
}