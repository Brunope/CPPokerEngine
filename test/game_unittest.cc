#include "Player.h"
#include "Action.h"
#include "Game.h"
#include "SimpleActor.h"
#include "TestActor.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <map>
#include <vector>

TEST(GameTest, Constructor) {
  uint32_t sb = 5;
  uint32_t bb = 10;
  Game game(sb, bb);
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
  Player player1;
  EXPECT_EQ(view.getPlayerByName("actor1", &player1), 0);
  EXPECT_EQ(player1.getName(), "actor1");
  
  game.addPlayer(&actor2, "actor2");
  EXPECT_EQ(view.getNumPlayers(), 2);
  Player player2;
  EXPECT_EQ(view.getPlayerByName("actor2", &player2), 0);
  EXPECT_EQ(player2.getName(), "actor2");

  game.removePlayer(player1);
  EXPECT_EQ(view.getNumPlayers(), 1);
  EXPECT_EQ(view.getPlayerByName("actor1", &player1), 1);
  EXPECT_EQ(view.getPlayerInSeat(player1.getSeat(), &player1), 1);
}

// button and small blind fold to the big blind, who should win the pot
// of 15, including his 10$ big blind, so net +5
TEST(GameTest, ThreePlayerInstaFold) {
  Game game(5, 10);
  const GameView &view = game.getView();
  TestActor actor0, actor1, actor2;
  game.addPlayer(&actor0, "p0", 100);
  game.addPlayer(&actor1, "p1", 100);
  game.addPlayer(&actor2, "p2", 100);
  size_t button_seat = view.getButtonPosition();
  actor0.queueAction(Action(FOLD));
  actor1.queueAction(Action(FOLD));
  actor2.queueAction(Action(FOLD));

  
  // play 1 hand
  game.play(1);

  // verify payout
  // winner is the big blind, 2 seats after the button
  Player button, sb, bb;
  EXPECT_EQ(view.getPlayerInSeat(button_seat, &button), 0);
  EXPECT_EQ(view.getPlayerInSeat(button_seat + 1, &sb), 0);  // ehhhh
  EXPECT_EQ(view.getPlayerInSeat(button_seat + 2, &bb), 0);
  EXPECT_EQ(button.getChips(), 100);
  EXPECT_EQ(sb.getChips(), 95);
  EXPECT_EQ(bb.getChips(), 105);

  // verify actions
  std::vector<Action> actions = view.getRoundAction();
  EXPECT_EQ(actions.size(), 4);
  EXPECT_EQ(actions[0].getType(), POST);
  EXPECT_EQ(actions[1].getType(), POST);
  EXPECT_EQ(actions[2].getType(), FOLD);
  EXPECT_EQ(actions[3].getType(), FOLD);
}
