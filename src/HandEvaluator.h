#ifndef HAND_EVALUATOR_H_
#define HAND_EVALUATOR_H_

#include <cinttypes>

// HandEvaluator uses a huge precomputed lookup table for speedy evals.
// It should be located right in the project base directory.
#define HAND_RANK_LOOKUP_FILE "HandRanks.dat"

class HandEvaluator {
public:
  HandEvaluator();

  int eval5(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) const;
private:
  FILE *f_;
  int HR_[32487834];
};

#endif  // HAND_EVALUATOR_H_
