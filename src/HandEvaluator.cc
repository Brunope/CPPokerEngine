#include <cstdio>

#include "HandEvaluator.h"

HandEvaluator::HandEvaluator() {
  f_ = fopen(HAND_RANK_LOOKUP_FILE, "r");
}

int HandEvaluator::eval5(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3,
                         uint8_t c4) const {
  return 0;
}
