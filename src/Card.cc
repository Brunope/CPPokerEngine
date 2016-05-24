#include <cstdint>
#include <string>
#include <cassert>

#include "Debug.h"
#include "Card.h"

Card::Card(uint8_t rk, uint8_t st) : rank(rk), suit(st) {
  if (DEBUG) {
    checkRep();
  }
}

bool operator==(const Card& lhs, const Card& rhs) {
  return lhs.rank == rhs.rank;
}

bool operator<(const Card& lhs, const Card& rhs) {
  return lhs.rank < rhs.rank;
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << c.str();
  return os;
}

void Card::checkRep() {
  assert((rank <= ACE && rank >= TWO) && (suit == HEARTS || suit == CLUBS ||
                                          suit == SPADES || suit == DIAMONDS));
}


// Nasty str() function, needs a ton of cases
const std::string Card::str() const {
  std::string rk, st;
  switch(rank) {
  case TWO:
    rk = "2";
    break;
  case THREE:
    rk = "3";
    break;
  case FOUR:
    rk = "4";
    break;
  case FIVE:
    rk = "5";
    break;
  case SIX:
    rk = "6";
    break;
  case SEVEN:
    rk = "7";
    break;
  case EIGHT:
    rk = "8";
    break;
  case NINE:
    rk = "9";
    break;
  case TEN:
    rk = "T";
    break;
  case JACK:
    rk = "J";
    break;
  case QUEEN:
    rk = "Q";
    break;
  case KING:
    rk = "K";
    break;
  case ACE:
    rk = "A";
    break;
  default:
    rk = "?";
    break;
  }
  
  switch (suit) {
  case CLUBS:
    st = "c";
    break;
  case HEARTS:
    st = "h";
    break;
  case SPADES:
    st = "s";
    break;
  case DIAMONDS:
    st = "d";
    break;
  default:
    st = "?";
    break;
  }

  return rk + st;
}

