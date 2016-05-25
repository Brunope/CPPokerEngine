#ifndef DECK_H_
#define DECK_H_

#include <vector>

#include "Card.h"

class Deck {
public:
  // Construct a Deck of 52 Cards (no jokers).
  Deck();
  
  // Return all dealt Cards to the Deck and shuffle them.
  void shuffle();

  // Return the top Card, and remove it from the Deck.
  // Deck must contain at least 1 Card.
  const Card dealNextCard();

  // Return a pair of the top two Cards on the Deck, and remove them.
  // Deck must contain at least 2 Cards
  const std::pair<Card, Card> dealHoleCards();

  friend class DeckTest;  // for unit tests
private:
  std::vector<Card> deck_;

  void constructDeck();
};

#endif  // DECK_H_
