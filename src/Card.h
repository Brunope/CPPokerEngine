#ifndef CARD_H_
#define CARD_H_

#include <cmath>
#include <cinttypes>
#include <string>
#include <iostream>

enum SUITS { CLUBS, DIAMONDS, SPADES, HEARTS };
enum RANKS { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK,
             QUEEN, KING, ACE };

class Card {
 public:
  const uint8_t rank;
  const uint8_t suit;

  // Construct a Card with suit st and rank rk.
  Card(uint8_t st, uint8_t rk);

  // Return a string representation of the Card in the form RANKsuit.
  // For example, a Card representating the two of hearts would
  // return '2h', and the ace of spades would return 'As'.
  const std::string str() const;

  /* Operator overloads */

  // Return true iff the ranks are equal, suit doesn't matter.
  friend bool operator==(const Card& lhs, const Card& rhs);

  // Return true iff the rank of the lhs is less than the rank of the rhs.
  friend bool operator<(const Card& lhs, const Card& rhs);

  // Append string representation from str() to os.
  friend std::ostream& operator<<(std::ostream& os, const Card& c);

 private:
  void checkRep();
};

#endif  // CARD_H_
