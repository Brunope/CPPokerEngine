#include <cstdlib>
#include <algorithm>
#include <vector>

#include "gtest/gtest.h"
#include "Card.h"
#include "Hand.h"

TEST(HandTest, IntCardWrapper) {
  Card c(ACE, SPADES);
  Card c2(TEN, HEARTS);
  Card c3(NINE, DIAMONDS);
  Card c4(TWO, CLUBS);
  EXPECT_EQ(UINT_CARD(c), 52);
  EXPECT_EQ(UINT_CARD(c2), 35);
  EXPECT_EQ(UINT_CARD(c3), 30);
  EXPECT_EQ(UINT_CARD(c4), 1);
}

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

TEST(HandTest, FiveCardStringConstructor) {
  std::vector<Card> cards = { Card(SIX, DIAMONDS),
                              Card(SIX, CLUBS),
                              Card(SIX, SPADES),
                              Card(SEVEN, DIAMONDS),
                              Card(SEVEN, SPADES) };
  Hand h("6d6c6s7d7s");
  EXPECT_TRUE(std::is_permutation(h.getCards().begin(), h.getCards().end(),
                                  cards.begin()));
}

TEST(HandTest, SevenCardStringConstructor) {
  std::vector<Card> cards = { Card(QUEEN, SPADES),
                              Card(FOUR, CLUBS),
                              Card(JACK, HEARTS),
                              Card(FOUR, HEARTS),
                              Card(NINE, SPADES) };
  Hand h("Qs3s4cJh4h8d9s");
  EXPECT_EQ(h.getCards(), cards);
  EXPECT_TRUE(std::is_permutation(h.getCards().begin(), h.getCards().end(),
                                  cards.begin()));
}

TEST(HandTest, HighCard) {
  Hand strong("Kc3s5d2cJs");
  Hand weak("Qc3h5c2sTh");
  EXPECT_GT(strong.eval(), weak.eval());
  strong = Hand("AcKhQsJh9c");
  weak = Hand("AhKsQhJd8h");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, Pair) {
  // pair > high card
  Hand strong("7c2s4d5d2c");
  Hand weak("Qs8cAcKh4d");
  EXPECT_GT(strong.eval(), weak.eval());

  // high pair > low pair
  strong = Hand("7c6sAh5dAs");
  weak = Hand("7hKc5s4cKs");
  EXPECT_GT(strong.eval(), weak.eval());

  // equal pair + high kicker > equal pair with low kicker
  strong = Hand("4hKd6sThTc");
  weak = Hand("TsTdKh6c2s");
  EXPECT_GT(strong.eval(), weak.eval());
  strong = Hand("Qs4cJh4h9s");
  weak = Hand("Qs4c4h8d9s");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, TwoPair) {
  // two pair > one pair
  Hand strong("5cQh5hAcQs");
  Hand weak("AsQdAc2c4d");
  EXPECT_GT(strong.eval(), weak.eval()) << strong.str() << ", " << weak.str();

  // top two pair > low two pair
  strong = Hand("7h8d7sKcKs");
  weak = Hand("Jh9c5h5sJd");
  EXPECT_GT(strong.eval(), weak.eval()) << strong.str() << ", " << weak.str();

  // equal two pair top kicker > equal two pair low kicker
  strong = Hand("3c3sJd2s2d");
  weak = Hand("3hTc3d2c2h");
  EXPECT_GT(strong.eval(), weak.eval()) << strong.str() << ", " << weak.str();
}

TEST(HandTest, ThreeOfaKind) {
  // three of a kind > two pair
  Hand strong("Qd5sQcQh7d");
  Hand weak("5cAh5h8sAd");
  EXPECT_GT(strong.eval(), weak.eval());

  // 3kind high > 3kind low
  strong = Hand("4c2sTdTcTh");
  weak = Hand("2c7h9d9c9h");
  EXPECT_GT(strong.eval(), weak.eval());

  // 3kind high kicker > 3kind low kicker
  strong = Hand("KhQdJcJsJh");
  weak = Hand("KsTcJcJsJh");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, Straight) {
  // straight > 3kind
  Hand strong("5c6h7s8h9c");
  Hand weak("4cThQsTdTs");
  EXPECT_GT(strong.eval(), weak.eval());

  // wheel straight
  strong = Hand("As2c3s5h4h");
  weak = Hand("AhAsAcKcQs");
  EXPECT_GT(strong.eval(), weak.eval());

  // high straight > low straight
  strong = Hand("QdKdJsTh9s");
  weak = Hand("QdJhTs9h8s");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, Flush) {
  // flush > straight
  Hand strong("7cJc6cQcKc");
  Hand weak("7sTs9c8hJc");
  EXPECT_GT(strong.eval(), weak.eval());

  // high flush > low flush
  strong = Hand("Td9d2d5dAd");
  weak = Hand("TdAd2d3d6d");
  EXPECT_GT(strong.eval(), weak.eval());
  weak = Hand("TdKd2d3d6d");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, FullHouse) {
  // full house > flush
  Hand strong("8d8hTc8sTh");
  Hand weak("5sQsAs3s2s");
  EXPECT_GT(strong.eval(), weak.eval());

  // high full house > low full house
  strong = Hand("JdKsKcJcKh");
  weak = Hand("QcQhJdJcQs");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, FourOfaKind) {
  // four of a kind > full house
  Hand strong("9cAs9h9d9s");
  Hand weak("4hQs4dQdQh");
  EXPECT_GT(strong.eval(), weak.eval());

  // high 4kind > low 4kind
  strong = Hand("QhQsQc2hQd");
  weak = Hand("9cJh9s9h9d");
  EXPECT_GT(strong.eval(), weak.eval());

  // equal 4kind high kicker > equal 4kind low kicker
  strong = Hand("6c6s6hAd6d");
  weak = Hand("6cKd6s6h6d");
  EXPECT_GT(strong.eval(), weak.eval());
}

TEST(HandTest, StraightFlush) {
  // straight flush > 4 of a kind
  Hand strong("4c5c6c7c8c");
  Hand weak("JcJhJsJd4s");
  EXPECT_GT(strong.eval(), weak.eval());

  // high straight flush > low straight flush
  strong = Hand("KsQsJsTs9s");
  weak = Hand("8s9sTsJsQs");
  EXPECT_GT(strong.eval(), weak.eval());

  // royal
  strong = Hand("JsQsTsAsKs");
  EXPECT_GT(strong.eval(), weak.eval());

  // wheel
  strong = Hand("5c4c3c2cAc");
  weak = Hand("TdTcTsAcTh");
  EXPECT_GT(strong.eval(), weak.eval());
}
