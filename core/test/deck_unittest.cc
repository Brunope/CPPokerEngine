#include <iostream>
#include <cstdlib>


#include "Card.h"
#include "Deck.h"
#include "gtest/gtest.h"

// Construct a Deck and ensure it has 52 unique Cards.
TEST(DeckTest, ConstructAndDeal) {
  Deck d;
  EXPECT_EQ(d.cardsLeft(), 52);
  std::vector<Card> seen;
  for (size_t i = 0; i < 52; i++) {
    Card c = d.dealNextCard();
    // make sure we haven't seen this Card yet
    for (size_t j = 0; j < seen.size(); j++) {
      EXPECT_FALSE(seen[j] == c) << seen[j] << ", " << c;
    }
    seen.push_back(c);
  }
  EXPECT_EQ(d.cardsLeft(), 0);
}

TEST(DeckTest, DealHoleCards) {
  Deck d;
  std::vector< std::pair<Card, Card> > seen;
  while (d.cardsLeft()) {
    std::pair<Card, Card> hc = d.dealHoleCards();
    for (std::pair<Card, Card> &hc_compare : seen) {
      EXPECT_FALSE(hc == hc_compare);
    }
    seen.push_back(hc);
  }
  EXPECT_EQ(d.cardsLeft(), 0);
}
