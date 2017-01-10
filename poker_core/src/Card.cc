#include <cstdint>
#include <string>
#include <cassert>
#include <iostream>
#include <map>

#include "Debug.h"
#include "Card.h"

std::map<uint8_t, std::string> Card::ranksuit_to_str = {
  { 0, "2c" }, { 4, "3c" }, { 8, "4c" }, { 12, "5c" }, { 16, "6c" },
  { 20, "7c" }, { 24, "8c" }, { 28, "9c" }, { 32, "Tc" }, { 36, "Jc" },
  { 40, "Qc" }, { 44, "Kc" }, { 48, "Ac" }, { 1, "2d" }, { 5, "3d" },
  { 9, "4d" }, { 13, "5d" }, { 17, "6d" }, { 21, "7d" }, { 25, "8d" },
  { 29, "9d" }, { 33, "Td" }, { 37, "Jd" }, { 41, "Qd" }, { 45, "Kd" },
  { 49, "Ad" }, { 2, "2h" }, { 6, "3h" }, { 10, "4h" }, { 14, "5h" },
  { 18, "6h" }, { 22, "7h" }, { 26, "8h" }, { 30, "9h" }, { 34, "Th" },
  { 38, "Jh" }, { 42, "Qh" }, { 46, "Kh" }, { 50, "Ah" }, { 3, "2s" },
  { 7, "3s" }, { 11, "4s" }, { 15, "5s" }, { 19, "6s" }, { 23, "7s" },
  { 27, "8s" }, { 31, "9s" }, { 35, "Ts" }, { 39, "Js" }, { 43, "Qs" },
  { 47, "Ks" }, { 51, "As" }
};

std::map<std::string, uint8_t> Card::str_to_ranksuit = {
  { "2c", 0 }, { "3c", 4 }, { "4c", 8 }, { "5c", 12 }, { "6c", 16 },
  { "7c", 20 }, { "8c", 24 }, { "9c", 28 }, { "Tc", 32 }, { "Jc", 36 },
  { "Qc", 40 }, { "Kc", 44 }, { "Ac", 48 }, { "2d", 1 }, { "3d", 5 },
  { "4d", 9 }, { "5d", 13 }, { "6d", 17 }, { "7d", 21 }, { "8d", 25 },
  { "9d", 29 }, { "Td", 33 }, { "Jd", 37 }, { "Qd", 41 }, { "Kd", 45 },
  { "Ad", 49 }, { "2h", 2 }, { "3h", 6 }, { "4h", 10 }, { "5h", 14 },
  { "6h", 18 }, { "7h", 22 }, { "8h", 26 }, { "9h", 30 }, { "Th", 34 },
  { "Jh", 38 }, { "Qh", 42 }, { "Kh", 46 }, { "Ah", 50 }, { "2s", 3 },
  { "3s", 7 }, { "4s", 11 }, { "5s", 15 }, { "6s", 19 }, { "7s", 23 },
  { "8s", 27 }, { "9s", 31 }, { "Ts", 35 }, { "Js", 39 }, { "Qs", 43 },
  { "Ks", 47 }, { "As", 51 }
};

Card::Card() : ranksuit_(0) {
  if (DEBUG) checkRep();
}

Card::Card(uint8_t rank, uint8_t suit) : ranksuit_(rank << 2 | suit) {
  if (DEBUG) checkRep();
}

Card::Card(const std::string ranksuit) {
  ranksuit_ = str_to_ranksuit[ranksuit];
}

uint8_t
Card::rank() const {
  return ranksuit_ >> 2;
}

uint8_t
Card::suit() const {
  return ranksuit_ & 0x3;
}  

const std::string
Card::str() const {
  return ranksuit_to_str[ranksuit_];
}

bool
operator==(const Card& lhs, const Card& rhs) {
  return lhs.ranksuit_ == rhs.ranksuit_;
}

std::ostream &
operator<<(std::ostream& os, const Card& c) {
  os << c.str();
  return os;
}

void
Card::checkRep() {
  assert((rank() <= ACE) && (suit() <= SPADES) && (ranksuit_ >> 6 == 0));
}

