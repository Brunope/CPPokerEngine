#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cinttypes>
#include <cassert>

#include "HandEvaluator.h"

#define HR_NUM_INTS 32487834  // defined by Ray's lookup algorithm
#define HR_FILE_BYTES 4 * HR_NUM_INTS  // 129951336

extern char *HandRanks_start asm("HandRanks_start");
extern char *HandRanks_end asm("HandRanks_end");

HandEvaluator::HandEvaluator() {
  uint32_t bytes_read =
    reinterpret_cast<uint64_t>(&HandRanks_end)
    - reinterpret_cast<uint64_t>(&HandRanks_start);
  if (bytes_read != HR_FILE_BYTES) {
    std::cerr << "Error: Read " << bytes_read << " bytes from HandRanks, \
expected " << HR_FILE_BYTES << std::endl;
    std::cerr << ", &HR_start: " << &HandRanks_start
              << ", &HR_end: " << &HandRanks_end << std::endl;
    exit(EXIT_FAILURE);
  }
  HR_ = reinterpret_cast<int *>(&HandRanks_start);
}

HandEvaluator::~HandEvaluator() {
  // delete[] HR_;
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
