#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

class Random {
public:
  static void seed();
  static void setSeed(uint seed);
  static uint getSeed();

  // return uniform random number between lo and hi, inclusive
  static int getInRange(int lo, int hi);

private:
  static std::mt19937 rng_;
  static uint seed_;
};

#endif  // RANDOM_H_
