#include "Player.h"
#include "Actions.h"
#include "Game.h"
#include "SimpleActor.h"
#include "gtest/gtest.h"

TEST(GameTest, Constructor) {
  uint32_t bb = 10;
  uint32_t sb = 5;
  Game game(bb, sb);
  SimpleActor actor;
  game.addPlayer(actor, "actor");
  EXPECT_EQ(game.getBigBlind(), bb);
  EXPECT_EQ(game.getSmallBlind(), sb);
  EXPECT_EQ(game.getNumPlayers(), 1);
}
  
