#ifndef HAND_H_
#define HAND_H_

#include <vector>

#include "Card.h"
#include "HandEvaluator.h"

// Wrapper to convert our cards to ints from 1 - 52 for HandEvaluator
#define UINT_CARD(x) x.rank() * 4 + x.suit() + 1

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
  // Hand shares a public HandEvaluator object that it uses to crunch the
  // evals. You can use it to evaluate large hand ranges faster directly,
  // since you don't have to construct a Hand object. You would then
  // pass each card as an argument to evaluator_'s eval* functions, eval5 or
  // eval7. HandEvaluators are very expensive to construct, so you should
  // use this one if you need one instead of making a new one.
  static const HandEvaluator evaluator_;
  
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

  // Return the str() of each Card in the Hand.
  const std::string str() const;
  
  /* Operator overloads */

  // Return true iff lhs evaluates to a strictly greater score than rhs.
  friend bool operator>(const Hand &lhs, const Hand &rhs);

  // Return true iff lhs evaluates to the same score as rhs.
  friend bool operator==(const Hand &lhs, const Hand &rhs);

private:
  std::vector<Card> hand_;
  int strength_;

  static const Hand findBest5CardHand(const std::vector<Card> &cards);
  
  void checkRep() const;
};

#endif  // HAND_H_
