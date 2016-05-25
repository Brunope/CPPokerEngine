#include <iostream>
#include <cstdlib>


#include "Card.h"
#include "Deck.h"
#include "gtest/gtest.h"

// Construct a Deck and ensure it has 52 unique Cards.
TEST(DeckTest, ConstructAndDeal) {
  Deck d;
  std::vector<Card> seen;
  for (size_t i = 0; i < 52; i++) {
    Card c = d.dealNextCard();
    // make sure we haven't seen this Card yet
    for (size_t j = 0; j < seen.size(); j++) {
      EXPECT_FALSE(seen[j] == c) << seen[j] << ", " << c;
    }
    seen.push_back(c);
  }
}
