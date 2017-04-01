#include <vector>

#include "Action.h"
#include "HandHistory.h"
#include "gtest/gtest.h"

TEST(HandHistoryTest, CopyAssignment) {
  HandHistory h1;

  h1.hand_action_[PREFLOP].push_back(Action(FOLD));
  h1.hand_action_[TURN].push_back(Action(CHECK));

  HandHistory h2 = HandHistory(h1);
  // should point to separate memory
  EXPECT_NE(h1.hand_action_, h2.hand_action_);

  EXPECT_EQ(h2.hand_action_[PREFLOP].size(), 1);
  EXPECT_EQ(h2.hand_action_[PREFLOP][0].getType(), FOLD);

  EXPECT_EQ(h2.hand_action_[TURN].size(), 1);
  EXPECT_EQ(h2.hand_action_[TURN][0].getType(), CHECK);

  EXPECT_EQ(h1.hand_action_[FLOP].size(), 0);
  EXPECT_EQ(h2.hand_action_[FLOP].size(), 0);
  EXPECT_EQ(h1.hand_action_[RIVER].size(), 0);
  EXPECT_EQ(h2.hand_action_[RIVER].size(), 0);

  h1.winner_ = Player("p");
  h2 = h1;
  EXPECT_NE(&(h1.winner_), &(h2.winner_));
  EXPECT_EQ(h2.winner_.getName(), h1.winner_.getName());
}
