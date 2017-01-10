#include "GameView.h"
#include "gtest/gtest.h"

TEST(GameViewTest, Simple) {
  GameView view;
  view.big_blind_ = 2;
  view.small_blind_ = 1;
  EXPECT_EQ(view.getBigBlind(), 2);
  EXPECT_EQ(view.getSmallBlind(), 1);
}
