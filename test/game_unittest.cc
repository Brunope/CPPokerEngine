#include "Player.h"
#include "Action.h"
#include "Game.h"
#include "SimpleActor.h"
#include "gtest/gtest.h"

TEST(GameTest, Constructor) {
  uint32_t bb = 10;
  uint32_t sb = 5;
  Game game(bb, sb);
  SimpleActor actor1, actor2;
  game.addPlayer(&actor1, "actor");
  const GameView &view = game.getView();
  EXPECT_EQ(view.getBigBlind(), bb);
  EXPECT_EQ(view.getSmallBlind(), sb);
  EXPECT_EQ(view.getNumPlayers(), 1);
}
  
TEST(GameTest, AddRemovePlayer) {
  Game game(5, 10);
  SimpleActor actor1, actor2;
  const GameView &view = game.getView();
  EXPECT_EQ(view.getNumPlayers(), 0);
  
  game.addPlayer(&actor1, "actor1");
  EXPECT_EQ(view.getNumPlayers(), 1);
  const Player *player1 = view.getPlayerByName("actor1");
  EXPECT_NE(player1, nullptr);
  EXPECT_EQ(player1->getName(), "actor1");
  
  game.addPlayer(&actor2, "actor2");
  EXPECT_EQ(view.getNumPlayers(), 2);
  const Player *player2 = view.getPlayerByName("actor2");
  EXPECT_NE(player2, nullptr);
  EXPECT_EQ(player2->getName(), "actor2");

  game.removePlayer(*player1);
  EXPECT_EQ(view.getNumPlayers(), 1);
  player1 = view.getPlayerByName("actor1");
  EXPECT_EQ(player1, nullptr);
}

TEST(GameTest, InstaFold) {
  
}
