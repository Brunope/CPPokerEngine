#include <cassert>
#include <string>
#include <sstream>

#include "Debug.h"
#include "HandEvaluator.h"
#include "Hand.h"

const HandEvaluator Hand::evaluator_;

Hand::Hand(const std::vector<Card> &cards) {
  if (cards.size() == 5) {
    hand_ = cards;
    strength_ = evaluator_.eval5(UINT_CARD(hand_[0]),
                                 UINT_CARD(hand_[1]),
                                 UINT_CARD(hand_[2]),
                                 UINT_CARD(hand_[3]),
                                 UINT_CARD(hand_[4]));
  } else {
    if (DEBUG) assert(cards.size() == 7);
    Hand best5cards = findBest5CardHand(hand_);
    *this = best5cards;
  }
  if (DEBUG) checkRep();
}

Hand::Hand(const std::string &cards) {
  if (DEBUG) assert(cards.size() == 10 || cards.size() == 14);
  std::vector<Card> new_cards;
  for (size_t i = 0; i < cards.size(); i += 2) {
    new_cards.push_back(Card(cards.substr(i, 2)));
  }
  *this = Hand(new_cards);
}

int Hand::eval() const {
  return strength_;
}

const std::vector<Card> &Hand::getCards() const {
  return hand_;
}

const std::string Hand::str() const {
  std::stringstream ss;
  for (const Card &c : hand_) {
    ss << c.str();
  }
  return ss.str();
}

bool operator>(const Hand &lhs, const Hand &rhs) {
  return lhs.strength_ > rhs.strength_;
}

bool operator==(const Hand &lhs, const Hand &rhs) {
  return lhs.strength_ == rhs.strength_;
}

void Hand::checkRep() const {
  assert(hand_.size() == 5);
  assert(strength_ > 0);  // even high card hand should eval to > 0
}

const Hand Hand::findBest5CardHand(const std::vector<Card> &cards) {
  if (cards.size() == 5) {
    return Hand(cards);
  }
  if (DEBUG) assert(cards.size() == 7);
  std::vector<Card> try_cards;
  std::vector<Card> best_cards(cards.begin(), cards.begin() + 5);
  Hand best_hand(best_cards);
  // gen all two unique combination of indices of cards to exclude
  for (size_t i = 0; i < cards.size() - 1; i++) {
    for (size_t j = i + 1; j < cards.size(); j++) {
      // Add remaining 5 to hand
      for (size_t k = 0; k < cards.size() && k != i && k != j; k++) {
        try_cards.push_back(cards[k]);
      }
      Hand try_hand(try_cards);
      if (try_hand > best_hand) {
        best_hand = try_hand;
      }
    }
  }
  return best_hand;
}
