#include "Actions.h"
#include "Player.h"
#include "gtest/gtest.h"

TEST(ActionTest, Raise) {
  Raise r(1);
  EXPECT_EQ(r.getType(), RAISE);
  EXPECT_EQ(r.getValue(), 1);
  Player p("me");
  Raise r2(&p, 2);
  EXPECT_EQ(r2.getValue(), 2);
  EXPECT_EQ(r2.getSource(), &p);
}

TEST(ActionTest, Call) {
  Call c(1);
  EXPECT_EQ(c.getType(), CALL);
  EXPECT_EQ(c.getValue(), 1);
  Player p("me");
  Call c2(1, &p);
  EXPECT_EQ(c2.getValue(), 1);
  EXPECT_EQ(c2.getSource(), &p);
}

TEST(ActionTest, Fold) {
  Player p("me");
  Fold f(&p);
  EXPECT_EQ(f.getType(), FOLD);
  EXPECT_EQ(f.getSource(), &p);
}

TEST(ActionTest, Check) {
  Player p("me");
  Check c(&p);
  EXPECT_EQ(c.getType(), CHECK);
  EXPECT_EQ(c.getSource(), &p);
}
