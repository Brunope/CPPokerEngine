#ifndef CARD_H_
#define CARD_H_

#include <cmath>
#include <cinttypes>
#include <string>
#include <iostream>
#include <boost/bimap.hpp>

// SUITS have arbitrary ordering - no suit has more value than the other -
// so the order of this enum doesn't matter.
enum SUITS { CLUBS, DIAMONDS, HEARTS, SPADES };

// RANKS, on the other hand, do have intrinsic value, ie ACE is greater
// than TEN. The enum is sorted in ascending order, so it is safe to
// compare two cards by rank to determine the higher card.
enum RANKS { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK,
             QUEEN, KING, ACE };

class Card {
public:
  // Construct a Card with suit st and rank rk.
  Card(uint8_t rk, uint8_t st);

  // Construct a Card matching the string ranksuit, which should contain 2
  // characters. The first specifies the rank, and should be a number for
  // cards 2 - 9, or the first letter of the rank (ie 'j' for "jack")
  // for ten - ace. The second specifies the suit - (c)lubs, (d)iamonds,
  // (h)earts, or (s)pades. For example, nine of hearts = "9h", and
  // queen of diamonds = "Qd".
  Card(const std::string ranksuit);
  
  // Copy constructor - set rank and suit equal to that of rhs
  Card(const Card &rhs);

  // Return a value from RANKS that is the rank of the Card.
  uint8_t rank() const;

  // Return a value from SUITS that is the suit of the Card.
  uint8_t suit() const;
  
  // Return a string in the format specified by the string constructor of
  // Card.
  const std::string str() const;

  /* Operator overloads */

  
  // Return true iff the ranks and suits are both equal
  friend bool operator==(const Card& lhs, const Card& rhs);

  // Append string representation from str() to os.
  friend std::ostream& operator<<(std::ostream& os, const Card& c);

private:
  uint8_t rank_;
  uint8_t suit_;

  static boost::bimap<uint8_t, char> rank_chars;
  static boost::bimap<uint8_t, char> suit_chars;
  
  void checkRep();
};

#endif  // CARD_H_
