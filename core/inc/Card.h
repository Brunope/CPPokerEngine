#ifndef CARD_H_
#define CARD_H_

#include <cmath>
#include <cinttypes>
#include <string>
#include <iostream>
#include <map>

// The following enums are declared for convenience. In C++11 (the
// current Makefile configuration) it is safe to assume enumerated
// values start at 0 and increment by 1.

// SUITS have arbitrary ordering - no suit has more value than the other.
// However, the order of this enum is still significant, Hand relies on
// clubs = 0, diamonds = 1, etc. for converting Cards to an integer format
// before evaluation.
enum SUITS : uint8_t { CLUBS, DIAMONDS, HEARTS, SPADES };

// RANKS, on the other hand, do have intrinsic value, ie ACE is greater
// than TEN. The enum is sorted in ascending order, so it is safe to
// compare two cards by rank to determine the higher card. Further,
// relying on C++11, it is guaranteed that TWO == 0, THREE == 1, ... ,
// ACE == 12. 
enum RANKS : uint8_t { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
                       TEN, JACK, QUEEN, KING, ACE };
/**
 * A Card is represented by two unsigned integers, rank and suit. The rank
 * is a value between TWO and ACE, and the suit is a value between CLUBS
 * and SPADES.
 */
class Card {
public:
  // Construct a Card with rank and suit 0, ie TWO of CLUBS. You shouldn't
  // invoke this constructor explicitly but it is provided to allow
  // declarations of std::vector or std::pair containing Cards.
  Card();
  
  // Construct a Card of rank and suit
  Card(uint8_t rank, uint8_t suit);

  // Construct a Card matching the string ranksuit, which should contain 2
  // characters. The first specifies the rank, and should be a number for
  // cards 2 - 9, or the first letter of the rank (ie 'J' for "jack")
  // for ten - ace. The second specifies the suit - (c)lubs, (d)iamonds,
  // (h)earts, or (s)pades. For example, nine of hearts = "9h", and
  // queen of diamonds = "Qd".
  Card(const std::string ranksuit);
  
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
  // The max possible rank of a Card is 12, and max suit is 3, so the rank
  // can be stored in 4 bits and the suit in 2. These bits are packed
  // into the 8 bit field ranksuit_ like so: --rrrrss. 
  uint8_t ranksuit_;

  static std::map<uint8_t, std::string> ranksuit_to_str;
  static std::map<std::string, uint8_t> str_to_ranksuit;
  
  void checkRep();
};

#endif  // CARD_H_
