#include "Action.h"
#include "Player.h"
#include "gtest/gtest.h"

TEST(ActionTest, Raise) {
  Action action(RAISE);
  EXPECT_EQ(action.getType(), RAISE);

  action = Action(RAISE, 1);
  EXPECT_EQ(action.getType(), RAISE);
  EXPECT_EQ(action.getAmount(), 1);
  
  Player p("me");
  action = Action(RAISE, 2, &p);
  EXPECT_EQ(action.getType(), RAISE);
  EXPECT_EQ(action.getAmount(), 2);
  EXPECT_EQ(action.getSource(), &p);
}

TEST(ActionTest, Call) {
  Action action(CALL);
  EXPECT_EQ(action.getType(), CALL);

  action = Action(CALL, 1);
  EXPECT_EQ(action.getType(), CALL);
  EXPECT_EQ(action.getAmount(), 1);
  
  Player p("me");
  action = Action(CALL, 2, &p);
  EXPECT_EQ(action.getType(), CALL);
  EXPECT_EQ(action.getAmount(), 2);
  EXPECT_EQ(action.getSource(), &p);
}

TEST(ActionTest, Fold) {
  Action action(FOLD);
  EXPECT_EQ(action.getType(), FOLD);

  action = Action(FOLD, 1);
  EXPECT_EQ(action.getType(), FOLD);
  
  Player p("me");
  action = Action(FOLD, 2, &p);
  EXPECT_EQ(action.getType(), FOLD);
  EXPECT_EQ(action.getSource(), &p);
}

TEST(ActionTest, Check) {
  Action action(CHECK);
  EXPECT_EQ(action.getType(), CHECK);

  action = Action(CHECK, 1);
  EXPECT_EQ(action.getType(), CHECK);
  
  Player p("me");
  action = Action(CHECK, 2, &p);
  EXPECT_EQ(action.getType(), CHECK);
  EXPECT_EQ(action.getSource(), &p);
}
