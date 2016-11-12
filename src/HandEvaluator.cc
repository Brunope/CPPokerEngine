#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cinttypes>

#include "HandEvaluator.h"

#define HR_NUM_INTS 32487834  // defined by Ray's lookup algorithm
#define HR_FILE_BYTES 4 * HR_NUM_INTS  // 129951336

HandEvaluator::HandEvaluator() {
  HR_ = new int[HR_NUM_INTS];
  memset(HR_, 0, HR_FILE_BYTES);
  FILE *f_ = fopen(HAND_RANK_LOOKUP_FILE, "rb");
  if (!f_) {
    std::cout << "Error, could not open hand rank data file. Make sure \
the filepath is correctly referenced by the HAND_RANK_LOOKUP_FILE constant \
in HandEvaluator.h" << std::endl;
    exit(EXIT_FAILURE);
  }
  size_t bytes = fread((void *)HR_, 1, HR_FILE_BYTES, f_);
  if (bytes != HR_FILE_BYTES) {
    std::cout << "Error, incorrect number of bytes read from hand rank data \
file. Expected " << HR_FILE_BYTES << ", got " << bytes << std::endl;
    exit(EXIT_FAILURE);
  }
  fclose(f_);
}

HandEvaluator::~HandEvaluator() {
  delete[] HR_;
}

int HandEvaluator::eval5(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3,
                         uint8_t c4) const {
  int strength = HR_[53 + c0];
  strength = HR_[strength + c1];
  strength = HR_[strength + c2];
  strength = HR_[strength + c3];
  strength = HR_[strength + c4];
  strength = HR_[strength];
  return strength;
}

int HandEvaluator::eval7(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3,
                         uint8_t c4, uint8_t c5, uint8_t c6) const {
  int strength = HR_[53 + c0];
  strength = HR_[strength + c1];
  strength = HR_[strength + c2];
  strength = HR_[strength + c3];
  strength = HR_[strength + c4];
  strength = HR_[strength + c5];
  strength = HR_[strength + c6];
  return strength;
}
