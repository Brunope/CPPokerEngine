#ifndef CARD_H_
#define CARD_H_

#include <cmath>
#include <cinttypes>
#include <string>
#include <iostream>

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

  // Copy constructor - set rank and suit equal to that of rhs
  Card(const Card &rhs);

  // Return a value from RANKS that is the rank of the Card.
  uint8_t rank() const;

  // Return a value from SUITS that is the suit of the Card.
  uint8_t suit() const;
  
  // Return a string representation of the Card in the form RANKsuit.
  // For example, a Card representating the two of hearts would
  // return '2h', and the ace of spades would return 'As'.
  const std::string str() const;

  /* Operator overloads */

  
  // Return true iff the ranks and suits are both equal
  friend bool operator==(const Card& lhs, const Card& rhs);

  // Append string representation from str() to os.
  friend std::ostream& operator<<(std::ostream& os, const Card& c);

private:
  uint8_t rank_;
  uint8_t suit_;

  void checkRep();
};

#endif  // CARD_H_
