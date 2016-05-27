#include <cstdint>
#include <string>
#include <cassert>
#include <iostream>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>

#include "Debug.h"
#include "Card.h"

// Initialize some bi-directional maps to help with converting Cards
// to and from string format. These maps are static private members of Card,
// so one instance is shared across all Cards, and can only be used by Card.
typedef boost::bimap<uint8_t, char> rs_map_t;
rs_map_t Card::rank_chars = boost::assign::list_of<rs_map_t::relation>
  (TWO, '2')
  (THREE, '3')
  (FOUR, '4')
  (FIVE, '5')
  (SIX, '6')
  (SEVEN, '7')
  (EIGHT, '8')
  (NINE, '9')
  (TEN, 'T')
  (JACK, 'J')
  (QUEEN, 'Q')
  (KING, 'K')
  (ACE, 'A');

rs_map_t Card::suit_chars = boost::assign::list_of<rs_map_t::relation>
  (CLUBS, 'c')
  (DIAMONDS, 'd')
  (HEARTS, 'h')
  (SPADES, 's');

Card::Card(uint8_t rk, uint8_t st) : rank_(rk), suit_(st) {
  if (DEBUG) checkRep();
}

Card::Card(const std::string ranksuit) {
  char rank = ranksuit[0];
  char suit = ranksuit[1];
  rank_ = rank_chars.right.at(rank);
  suit_ = suit_chars.right.at(suit);
}

Card::Card(const Card& rhs) : rank_(rhs.rank()), suit_(rhs.suit()) {
  if (DEBUG) checkRep();
}

uint8_t Card::rank() const {
  return rank_;
}

uint8_t Card::suit() const {
  return suit_;
}

  

bool operator==(const Card& lhs, const Card& rhs) {
  return (lhs.rank() == rhs.rank()) && (lhs.suit() == rhs.suit());
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << c.str();
  return os;
}

void Card::checkRep() {
  assert((rank_ <= ACE && rank_ >= TWO) &&
         (suit_ == HEARTS || suit_ == CLUBS ||
          suit_ == SPADES || suit_ == DIAMONDS));
}

const std::string Card::str() const {
  char rank = rank_chars.left.at(rank_);
  char suit = suit_chars.left.at(suit_);
  std::stringstream ss;
  ss << rank << suit;
  return ss.str();
}

