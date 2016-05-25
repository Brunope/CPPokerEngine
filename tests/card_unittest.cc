#include "Card.h"
#include "gtest/gtest.h"

TEST(CardTest, Constructor) {
  Card c1(TWO, CLUBS);
  Card c2(JACK, HEARTS);
  EXPECT_EQ(c1.rank(), TWO);
  EXPECT_EQ(c1.suit(), CLUBS);
  EXPECT_EQ(c2.rank(), JACK);
  EXPECT_EQ(c2.suit(), HEARTS);
}

TEST(CardTest, CopyConstructor) {
  Card c1(TEN, HEARTS);
  Card c2(c1);
  EXPECT_EQ(c1, c2);
}

TEST(CardTest, Assignment) {
  Card c1(FOUR, CLUBS);
  Card c2 = c1;
  EXPECT_EQ(c1, c2);
  EXPECT_EQ(c2.rank(), FOUR);
  EXPECT_EQ(c2.suit(), CLUBS);
}

TEST(CardTest, String) {
  Card c1(SEVEN, DIAMONDS);
  Card c2(TEN, CLUBS);
  Card c3(JACK, HEARTS);
  Card c4(QUEEN, SPADES);
  Card c5(KING, CLUBS);
  Card c6(ACE, SPADES);
  EXPECT_EQ(c1.str(), "7d");
  EXPECT_EQ(c2.str(), "Tc");
  EXPECT_EQ(c3.str(), "Jh");
  EXPECT_EQ(c4.str(), "Qs");
  EXPECT_EQ(c5.str(), "Kc");
  EXPECT_EQ(c6.str(), "As");
}

TEST(CardTest, Equal) {
  Card c1(QUEEN, HEARTS);
  Card c2(QUEEN, SPADES);
  Card c3(KING, HEARTS);
  Card c4(KING, HEARTS);
  Card c5(QUEEN, SPADES);
  EXPECT_FALSE(c1 == c2);
  EXPECT_FALSE(c1 == c3);
  EXPECT_EQ(c2, c5);
  EXPECT_EQ(c3, c3);
  c1 = c2;
  EXPECT_EQ(c1, c2);
}
