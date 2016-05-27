#include <cstdlib>
#include <vector>

#include "gtest/gtest.h"
#include "Card.h"
#include "Hand.h"

TEST(HandTest, FiveCardConstructor) {
  std::vector<Card> cards;
  cards.push_back(Card(TWO, CLUBS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(KING, SPADES));
  cards.push_back(Card(SEVEN, SPADES));

  Hand hand(cards);
  EXPECT_EQ(hand.getCards(), cards);
  EXPECT_GE(hand.eval(), 0);  // not so rigorous right now but we'll get there
}

TEST(HandTest, SevenCardConstructor) {
  std::vector<Card> cards;
  cards.push_back(Card(TEN, DIAMONDS));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(NINE, HEARTS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(THREE, DIAMONDS));
  cards.push_back(Card(SIX, DIAMONDS));
  cards.push_back(Card(SEVEN, DIAMONDS));

  Hand hand(cards);
  // remove everything but the flush
  cards.erase(cards.begin() + 2);
  cards.erase(cards.begin() + 2);
  EXPECT_EQ(hand.getCards(), cards);
}

TEST(HandTest, StringConstructor) {
  std::vector<Card> cards = { Card("6c"), Card("6c") };
}

// Verify a hand with higher kickers wins against other high card hands
TEST(HandTest, HighCard) {
}

// Verify a pair is stronger than a high card
TEST(HandTest, Pair) {
}

// Verify a higher pair is stronger than a lower pair
TEST(HandTest, TopPair) {
}

// Verify two-pair beats a pair
TEST(HandTest, TwoPair) {
}

// Verify three of a king beats two-pair
TEST(HandTest, ThreeOfaKind) {
}

// Verify straight beats three of a kind
TEST(HandTest, Straight) {
}

// Verify flush beats straight
TEST(HandTest, Flush) {
}

// Verify full house beats flush
TEST(HandTest, FullHouse) {
}

// Verify four of a kind beats full house
TEST(HandTest, FourOfaKind) {
}

// Verify straight flush beats four of a kind
TEST(HandTest, StraightFlush) {
}

// Verify royal flush beats straight flush
TEST(HandTest, RoyalFlush) {
}
