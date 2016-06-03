#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cinttypes>

#include "HandEvaluator.h"

HandEvaluator::HandEvaluator() {
  uint32_t HR_size = 4 * 32487834;
  HR_ = new int[32487834];
  memset(HR_, 0, HR_size);
  FILE *f_ = fopen(HAND_RANK_LOOKUP_FILE, "rb");
  if (!f_) {
    std::cout << "Error, could not open hand rank data file. Make sure \
the filepath is correctly referenced by the HAND_RANK_LOOKUP_FILE constant \
in HandEvaluator.h" << std::endl;
    exit(EXIT_FAILURE);
  }
  size_t bytes = fread((void*)HR_, 1, HR_size, f_);
  if (bytes != HR_size) {
    std::cout << "Error, incorrect number of bytes read from hand rank data \
file. Expected " << HR_size << ", got " << bytes << std::endl;
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
