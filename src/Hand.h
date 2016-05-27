#ifndef HAND_H_
#define HAND_H_

#include <vector>

#include "Card.h"
#include "HandEvaluator.h"

// Convert a Card x to an integer in the range [1, 52] inclusive. Cards
// are grouped by suit, ie 1 - 13 are of the same suit, 14 - 26 are of the
// same suit, etc. 13 would be an Ace, and 1 would be a Two. This is necessary
// for converting our Card representation to a form useable by the
// HandEvaluator, which just treats cards as unsigned integers within the
// above specified range.
#define UINT_CARD(x) x.suit() * 12 + x.rank() + 1

/**
 * A Hand is a collection of 5 Cards representing a poker hand. It can be
 * quickly be evaluated via Hand::eval() for an integer score of strength,
 * or compared with == and <. A greater Hand beats a lesser Hand.
 *
 * For more information on poker hands, see 
 * https://en.wikipedia.org/wiki/List_of_poker_hands
 * 
 * Hand rank does not depend on suit. A flush of one suit will not beat
 * a flush of another suit; their eval() results will be equal. 
 */
class Hand {
public:
  // Construct a Hand of the given Cards. If 7 Cards are provided, the 5
  // Cards forming the best Hand will be selected. cards must contain
  // either 5 or 7 Cards.
  Hand(const std::vector<Card> &cards);

  // Construct a Hand from the string cards, which should contain
  // subsequent pairs of characters, each pair specifying a Card
  // in the format described by the Card string constructor - see Card.h
  // cards should be either 10 characters, for a 5 card hand, or 14
  // characters, for a 7 card hand. For example, Hand("AsAhAd9c9d") would
  // construct a 5 card hand containing the ace of spades, ace of hearts,
  // ace of diamonds, nine of clubs, and nine of diamonds.
  Hand(const std::string &cards);
  
  // Return an integer score of the Hand representing its total evaluated
  // strength. A higher score is better.
  int eval() const;

  // Return the 5 Cards that make up the Hand.
  const std::vector<Card> &getCards() const;

  // Return the type of Hand followed by kickers
  const std::string str() const;
  
  /* Operator overloads */

  // Return true iff lhs evaluates to a strictly greater score than rhs.
  friend bool operator>(const Hand &lhs, const Hand &rhs);

  // Return true iff lhs evaluates to the same score as rhs.
  friend bool operator==(const Hand &lhs, const Hand &rhs);

private:
  std::vector<Card> hand_;
  int strength_;

  static const HandEvaluator evaluator_;  // what actually crunches numbers

  static const Hand findBest5CardHand(const std::vector<Card> &cards);
  
  void checkRep() const;
};

#endif  // HAND_H_
