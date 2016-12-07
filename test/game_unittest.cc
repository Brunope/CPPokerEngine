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
  EXPECT_EQ(view.getPlayerBySeat(player1.getSeat(), &player1), 1);
}

// With just two players, the button is actually the small blind and
// first to act preflop, and the other player is the big blind
TEST(GameTest, TwoPlayerBlinds) {
  Game game(5, 10);
  const GameView &view = game.getView();
  TestActor actor0, actor1;

  size_t button_seat = view.getButtonPosition();
  game.addPlayer(&actor0, "p0", 100);
  game.addPlayer(&actor1, "p1", 100);
  actor0.queueAction(Action(FOLD));
  actor1.queueAction(Action(FOLD));
  game.play(1);

  Player button, big_blind;
  EXPECT_EQ(view.getPlayerBySeat(button_seat, &button), 0);
  big_blind = view.getNextPlayer(button);

  // button should have folded small blind to the big blind who wins 15
  std::vector<Action> hand_action[NUM_ACTIONS];
  EXPECT_EQ(view.getHandAction(hand_action), 0);
  EXPECT_EQ(hand_action[PREFLOP].size(), 3);
  Action sbpost = hand_action[PREFLOP][0];
  Action bbpost = hand_action[PREFLOP][1];
  Action sbfold = hand_action[PREFLOP][2];
  EXPECT_EQ(sbpost.getSource()->getName(), button.getName());
  EXPECT_EQ(sbpost.getAmount(), 5);
  EXPECT_EQ(bbpost.getSource()->getName(), big_blind.getName());
  EXPECT_EQ(bbpost.getAmount(), 10);
  EXPECT_EQ(sbfold.getSource()->getName(), button.getName());

  EXPECT_EQ(button.getChips(), 95);
  EXPECT_EQ(big_blind.getChips(), 105);
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
  EXPECT_EQ(view.getPlayerBySeat(button_seat, &button), 0);
  EXPECT_EQ(view.getPlayerBySeat(button_seat + 1, &sb), 0);  // ehhhh
  EXPECT_EQ(view.getPlayerBySeat(button_seat + 2, &bb), 0);
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

// Raise preflop, 1 player folds, other 2 call, 3 to flop, check
// raise fold fold, win hand
TEST(GameTest, FourPlayerFlopFold) {
  Game game(5, 10);
  const GameView &view = game.getView();
  TestActor a0, a1, a2, a3;
  game.addPlayer(&a0, "p0", 100);  // assume a0/p0 at seat 0, button
  game.addPlayer(&a1, "p1", 100);
  game.addPlayer(&a2, "p2", 100);
  game.addPlayer(&a3, "p3", 100);

  // a1 posts small blind, a2 posts big, a3 raises to 20
  a3.queueAction(Action(RAISE, 20));
  // a0 folds, a1 calls, a2 calls
  a0.queueAction(Action(FOLD));
  a1.queueAction(Action(CALL, 15));
  a2.queueAction(Action(CALL, 10));
  // flop comes, a1 first to act, checks
  a1.queueAction(Action(CHECK));
  // a2 bets 30
  a2.queueAction(Action(RAISE, 30));
  // a3 folds, a1 folds, a2 wins 90, net +40
  a3.queueAction(Action(FOLD));
  a1.queueAction(Action(FOLD));

  // do it!!
  game.play(1);

  Player p0, p1, p2, p3;
  EXPECT_EQ(view.getPlayerBySeat(0, &p0), 0);
  EXPECT_EQ(view.getPlayerBySeat(1, &p1), 0);
  EXPECT_EQ(view.getPlayerBySeat(2, &p2), 0);
  EXPECT_EQ(view.getPlayerBySeat(3, &p3), 0);

  EXPECT_EQ(p0.getChips(), 100);
  EXPECT_EQ(p1.getChips(), 80);
  EXPECT_EQ(p2.getChips(), 140);
  EXPECT_EQ(p3.getChips(), 80);

  std::vector<Action> hand_action[NUM_ACTIONS];
  EXPECT_EQ(view.getHandAction(hand_action), 0);
  
  EXPECT_EQ(hand_action[PREFLOP].size(), 6);
  EXPECT_EQ(hand_action[PREFLOP][0].getType(), POST);
  EXPECT_EQ(hand_action[PREFLOP][1].getType(), POST);
  EXPECT_EQ(hand_action[PREFLOP][2].getType(), RAISE);
  EXPECT_EQ(hand_action[PREFLOP][2].getAmount(), 20);
  EXPECT_EQ(hand_action[PREFLOP][3].getType(), FOLD);
  EXPECT_EQ(hand_action[PREFLOP][4].getType(), CALL);
  EXPECT_EQ(hand_action[PREFLOP][4].getAmount(), 15);
  EXPECT_EQ(hand_action[PREFLOP][5].getType(), CALL);
  EXPECT_EQ(hand_action[PREFLOP][5].getAmount(), 10);

  EXPECT_EQ(hand_action[FLOP].size(), 4);
  EXPECT_EQ(hand_action[FLOP][0].getType(), CHECK);
  EXPECT_EQ(hand_action[FLOP][1].getType(), RAISE);
  EXPECT_EQ(hand_action[FLOP][1].getAmount(), 30);
  EXPECT_EQ(hand_action[FLOP][2].getType(), FOLD);
  EXPECT_EQ(hand_action[FLOP][2].getType(), FOLD);
}
