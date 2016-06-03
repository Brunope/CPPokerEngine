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

Card::Card(uint8_t rank, uint8_t suit) : ranksuit_(rank << 2 | suit) {
  if (DEBUG) checkRep();
}

Card::Card(const std::string ranksuit) {
  char r = ranksuit[0];
  char s = ranksuit[1];
  ranksuit_ = rank_chars.right.at(r) << 2 | suit_chars.right.at(s);
}

uint8_t Card::rank() const {
  return ranksuit_ >> 2;
}

uint8_t Card::suit() const {
  return ranksuit_ & 0x3;
}

  

bool operator==(const Card& lhs, const Card& rhs) {
  return (lhs.rank() == rhs.rank()) && (lhs.suit() == rhs.suit());
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << c.str();
  return os;
}

void Card::checkRep() {
  assert((rank() <= ACE) && (suit() <= SPADES) && (ranksuit_ >> 6 == 0));
}

const std::string Card::str() const {
  char r = rank_chars.left.at(rank());
  char s = suit_chars.left.at(suit());
  std::stringstream ss;
  ss << r << s;
  return ss.str();
}

