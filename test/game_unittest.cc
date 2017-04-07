#include <cstdio>
#include <map>
#include <vector>

#include "Player.h"
#include "Action.h"
#include "Game.h"
#include "SimpleAgent.h"
#include "GameView.h"
#include "HandHistory.h"
#include "TestAgent.h"
#include "RandomAgent.h"
#include "TightAgent.h"
#include "gtest/gtest.h"

TEST(GameTest, Constructor) {
  uint32_t sb = 5;
  uint32_t bb = 10;
  Game game(sb, bb);
  std::shared_ptr<SimpleAgent> agent1(new SimpleAgent());
  std::shared_ptr<SimpleAgent> agent2(new SimpleAgent());
  game.addPlayer(agent1, "agent");
  std::shared_ptr<const GameView> view = game.getView();
  EXPECT_EQ(view->getBigBlind(), bb);
  EXPECT_EQ(view->getSmallBlind(), sb);
  EXPECT_EQ(view->getNumPlayers(), 1);
}
  
TEST(GameTest, AddRemovePlayer) {
  Game game(5, 10);
  std::shared_ptr<SimpleAgent> agent1(new SimpleAgent());
  std::shared_ptr<SimpleAgent> agent2(new SimpleAgent());
  std::shared_ptr<const GameView> view = game.getView();
  EXPECT_EQ(view->getNumPlayers(), 0);
  
  game.addPlayer(agent1, "agent1");
  EXPECT_EQ(view->getNumPlayers(), 1);
  Player player1;
  EXPECT_EQ(view->getPlayerByName("agent1", &player1), 0);
  EXPECT_EQ(player1.getName(), "agent1");
  
  game.addPlayer(agent2, "agent2");
  EXPECT_EQ(view->getNumPlayers(), 2);
  Player player2;
  EXPECT_EQ(view->getPlayerByName("agent2", &player2), 0);
  EXPECT_EQ(player2.getName(), "agent2");

  game.removePlayer(player1.getSeat());
  EXPECT_EQ(view->getNumPlayers(), 1);
  EXPECT_EQ(view->getPlayerByName("agent1", &player1), 1);
  EXPECT_EQ(view->getPlayerBySeat(player1.getSeat(), &player1), 1);
}

// With just two players, the button is actually the small blind and
// first to act preflop, and the other player is the big blind
TEST(GameTest, TwoPlayerBlinds) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> agent0(new TestAgent());
  std::shared_ptr<TestAgent> agent1(new TestAgent());

  size_t button_seat = view->getButtonPosition();
  game.addPlayer(agent0, "p0", 100);
  game.addPlayer(agent1, "p1", 100);
  agent0->queueAction(Action(FOLD));
  agent1->queueAction(Action(FOLD));

  game.play(1);

  Player button, big_blind;
  EXPECT_EQ(view->getPlayerBySeat(button_seat, &button), 0);
  big_blind = view->getNextPlayer(button);

  // button should have folded small blind to the big blind who wins 15
  const HandHistory hh = view->getHandHistory();
  // const std::vector<Action> *hand_action =
  //   view->getHandHistory().getHandAction();
  const std::vector<Action> *hand_action = hh.getHandAction();
  const std::vector<Action> round_action =
    view->getHandHistory().getRoundAction(PREFLOP);

  EXPECT_EQ(round_action.size(), 3);
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
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> agent0(new TestAgent());
  std::shared_ptr<TestAgent> agent1(new TestAgent());
  std::shared_ptr<TestAgent> agent2(new TestAgent());
  game.addPlayer(agent0, "p0", 100);
  game.addPlayer(agent1, "p1", 100);
  game.addPlayer(agent2, "p2", 100);
  size_t button_seat = view->getButtonPosition();
  agent0->queueAction(Action(FOLD));
  agent1->queueAction(Action(FOLD));
  agent2->queueAction(Action(FOLD));
  
  // play 1 hand
  game.play(1);

  // verify payout
  // winner is the big blind, 2 seats after the button
  Player button, sb, bb;
  EXPECT_EQ(view->getPlayerBySeat(button_seat, &button), 0);
  EXPECT_EQ(view->getPlayerBySeat(button_seat + 1, &sb), 0);  // ehhhh
  EXPECT_EQ(view->getPlayerBySeat(button_seat + 2, &bb), 0);
  EXPECT_EQ(button.getChips(), 100);
  EXPECT_EQ(sb.getChips(), 95);
  EXPECT_EQ(bb.getChips(), 105);

  // verify actions
  std::vector<Action> actions = view->getHandHistory().getRoundAction(PREFLOP);
  EXPECT_EQ(view->getHandHistory().multipleWinners(), false);
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
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  std::shared_ptr<TestAgent> a2(new TestAgent);
  std::shared_ptr<TestAgent> a3(new TestAgent);
  game.addPlayer(a0, "p0", 100);  // assume a0/p0 at seat 0, button
  game.addPlayer(a1, "p1", 100);
  game.addPlayer(a2, "p2", 100);
  game.addPlayer(a3, "p3", 100);

  // a1 posts small blind, a2 posts big, a3 raises to 20
  a3->queueAction(Action(RAISE, 20));
  // a0 folds, a1 calls, a2 calls
  a0->queueAction(Action(FOLD));
  a1->queueAction(Action(CALL, 15));
  a2->queueAction(Action(CALL, 10));
  // flop comes, a1 first to act, checks
  a1->queueAction(Action(CHECK));
  // a2 bets 30
  a2->queueAction(Action(RAISE, 30));
  // a3 folds, a1 folds, a2 wins 90, net +40
  a3->queueAction(Action(FOLD));
  a1->queueAction(Action(FOLD));

  // do it!!
  game.play(1);

  HandHistory hh  = view->getHandHistory();
  if (!hh.multipleWinners()) {
    Player p0, p1, p2, p3;
    EXPECT_EQ(view->getPlayerBySeat(0, &p0), 0);
    EXPECT_EQ(view->getPlayerBySeat(1, &p1), 0);
    EXPECT_EQ(view->getPlayerBySeat(2, &p2), 0);
    EXPECT_EQ(view->getPlayerBySeat(3, &p3), 0);

    EXPECT_EQ(p0.getChips(), 100);
    EXPECT_EQ(p1.getChips(), 80);
    EXPECT_EQ(p2.getChips(), 140);
    EXPECT_EQ(p3.getChips(), 80);
  }
  // can't do this, WHY?
  // const std::vector<Action> *hand_action =
  //   view->getHandHistory().getHandAction();  

  const std::vector<Action> *hand_action = hh.getHandAction();
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

TEST(GameTest, BigBlindRaiseOptionCheckToShowdown) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  std::shared_ptr<TestAgent> a2(new TestAgent);
  std::shared_ptr<TestAgent> a3(new TestAgent);
  
  game.addPlayer(a0, "p0", 100);
  game.addPlayer(a1, "p1", 100);
  game.addPlayer(a2, "p2", 100);
  game.addPlayer(a3, "p3", 100);

  Player p0, p1, p2, p3;
  // need p0 in seat 0
  EXPECT_EQ(view->getPlayerBySeat(0, &p0), 0);
  EXPECT_EQ(p0.getName(), "p0");
  
  // a1 post sb, a2 post bb
  a3->queueAction(Action(CALL, 10));
  a0->queueAction(Action(CALL, 10));
  a1->queueAction(Action(CALL, 5));
  a2->queueAction(Action(RAISE, 20));
  // everyone has to call 10 more to see a flop
  a3->queueAction(Action(CALL, 10));
  a0->queueAction(Action(CALL, 10));
  a1->queueAction(Action(CALL, 10));
  // deal flop, now everyone check starting with a1
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(CHECK));
  a3->queueAction(Action(CHECK));
  a0->queueAction(Action(CHECK));
  // deal turn
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(CHECK));
  a3->queueAction(Action(CHECK));
  a0->queueAction(Action(CHECK));
  // river
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(CHECK));
  a3->queueAction(Action(CHECK));
  a0->queueAction(Action(CHECK));
  // showdown! check who wins

  game.play(1);

  const HandHistory& history = view->getHandHistory();

  // forget about cases with chopped pot
  if (!history.multipleWinners()) {
    EXPECT_EQ(history.multipleWinners(), false);
    Player winner = history.getWinner();
    ASSERT_TRUE(winner.getSeat() <= 3);

    std::map<size_t, Player> players = view->getPlayers();
    for (auto it = players.begin(); it != players.end(); ++it) {
      if (it->second.getSeat() == winner.getSeat()) {
        EXPECT_EQ(it->second.getName(), winner.getName());
        // should have won pot of 80, net +60
        EXPECT_EQ(it->second.getChips(), 160);
        EXPECT_EQ(winner.getChips(), 160);
      } else {
        EXPECT_EQ(it->second.getChips(), 80);
      }
    }
  } else {
    std::cout << "skipping checks because pot chopped" << std::endl;
  }
  
  const std::vector<Action> *actions = history.getHandAction();
  EXPECT_EQ(actions[PREFLOP].size(), 9);
}

// Raise - reraise - call on every street up to showdown
TEST(GameTest, FivePlayerBetRaiseCallToShowdown) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  std::shared_ptr<TestAgent> a2(new TestAgent);
  std::shared_ptr<TestAgent> a3(new TestAgent);
  std::shared_ptr<TestAgent> a4(new TestAgent);
  
  game.addPlayer(a0, "p0", 1000);  // lil extra chips
  game.addPlayer(a1, "p1", 1000);
  game.addPlayer(a2, "p2", 1000);
  game.addPlayer(a3, "p3", 1000);
  game.addPlayer(a4, "p4", 1000);

  // preflop
  a3->queueAction(Action(RAISE, 30));
  a4->queueAction(Action(CALL, 30));
  a0->queueAction(Action(CALL, 30));
  a1->queueAction(Action(CALL, 25));
  a2->queueAction(Action(RAISE, 60));
  a3->queueAction(Action(CALL, 30));
  a4->queueAction(Action(CALL, 30));
  a0->queueAction(Action(CALL, 30));
  a1->queueAction(Action(CALL, 30));

  // flop
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(CHECK));
  a3->queueAction(Action(CHECK));
  a4->queueAction(Action(CHECK));
  a0->queueAction(Action(RAISE, 25));
  a1->queueAction(Action(CALL, 25));
  a2->queueAction(Action(RAISE, 75));
  a3->queueAction(Action(CALL, 75));
  a4->queueAction(Action(CALL, 75));
  a0->queueAction(Action(CALL, 50));
  a1->queueAction(Action(CALL, 50));

  // turn
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(RAISE, 100));
  a3->queueAction(Action(RAISE, 300));
  a4->queueAction(Action(CALL, 300));
  a0->queueAction(Action(CALL, 300));
  a1->queueAction(Action(CALL, 300));
  a2->queueAction(Action(CALL, 200));

  // river
  a1->queueAction(Action(RAISE, 200));
  a2->queueAction(Action(RAISE, 500));
  a3->queueAction(Action(CALL, 500));
  a4->queueAction(Action(CALL, 500));
  a0->queueAction(Action(CALL, 500));
  a1->queueAction(Action(CALL, 300));

  game.play(1);


  const HandHistory& history = view->getHandHistory();
  Player winner = history.getWinner();
  const std::map<size_t, uint32_t> winnings = history.getPlayerWinnings();
  std::map<size_t, Player> players = view->getPlayers();

  if (!history.multipleWinners()) {
    ASSERT_FALSE(history.multipleWinners());
    ASSERT_TRUE(winner.getSeat() <= 4);

    for (auto it = players.begin(); it != players.end(); ++it) {
      if (it->second.getSeat() == winner.getSeat()) {
        EXPECT_EQ(it->second.getName(), winner.getName());
        // each player contributed total of 935
        // should have won pot of 4675, net +3740
        EXPECT_EQ(it->second.getChips(), 4740);
        EXPECT_EQ(winner.getChips(), 4740);
      } else {
        EXPECT_EQ(it->second.getChips(), 65);
      }
    }
  } else {
    // some sort of chop
    uint32_t exp_winnings = 4675 / winnings.size();
    uint32_t exp_chips = exp_winnings + 1000 - 935;
    for (auto const &kv : players) {
      size_t seat = kv.first;
      const Player &p = kv.second;
      if (winnings.count(seat)) {
        // player won part of the chop
        EXPECT_EQ(winnings.at(seat), exp_winnings);
        EXPECT_EQ(p.getChips(), exp_chips);
      } else {
        EXPECT_EQ(winnings.count(seat), 0);
        EXPECT_EQ(p.getChips(), 1000 - 935);
      }
    }
  }

  const std::vector<Action> *actions = history.getHandAction();
  EXPECT_EQ(actions[PREFLOP].size(), 11);
  EXPECT_EQ(actions[FLOP].size(), 11);
  EXPECT_EQ(actions[TURN].size(), 7);
  EXPECT_EQ(actions[RIVER].size(), 6);
}

TEST(GameTest, TwoPlayerSingleAllIn) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  game.addPlayer(a0, "p0", 100);
  game.addPlayer(a1, "p1", 100);

  a0->queueAction(Action(CALL, 5));
  a1->queueAction(Action(RAISE, 100));
  a0->queueAction(Action(CALL, 90));

  // play one hand, remove the losing player at end of hand unless chop
  game.play(1);

  Player p0, p1;
  
  const HandHistory& hh = view->getHandHistory();
  EXPECT_EQ(hh.getRoundAction(FLOP).size(), 0);
  EXPECT_EQ(hh.getRoundAction(TURN).size(), 0);
  EXPECT_EQ(hh.getRoundAction(RIVER).size(), 0);

  if (!hh.multipleWinners()) {
    Player winner = hh.getWinner();
    EXPECT_EQ(winner.getChips(), 200);
    if (winner.getSeat() == 0) {
      EXPECT_EQ(view->getPlayerBySeat(0, &p0), 0);
      // p1 shouldn't exist in game anymore
      EXPECT_EQ(view->getPlayerBySeat(1, &p1), 1);
      EXPECT_EQ(p0.getName(), winner.getName());
      EXPECT_EQ(p0.getChips(), 200);
    } else {
      EXPECT_EQ(winner.getSeat(), 1);
      // p0 shouldn't exist in game anymore
      EXPECT_EQ(view->getPlayerBySeat(0, &p0), 1);
      EXPECT_EQ(view->getPlayerBySeat(1, &p1), 0);
      EXPECT_EQ(p1.getName(), winner.getName());
      EXPECT_EQ(p1.getChips(), 200);
    }
  } else {
    std::cout << "skipping checks because pot chopped" << std::endl;
  }
}

// Three players, short stack all in and wins, big stack
// wins side pot, hand 0, seed 2393147711
TEST(GameTest, ThreePlayerSingleAllInSidePot) {
  Game game(5, 10);
  Random::setSeed(2393147711);

  std::shared_ptr<const GameView> view = game.getView();
  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  std::shared_ptr<TestAgent> a2(new TestAgent);
  game.addPlayer(a0, "p0", 100);
  game.addPlayer(a1, "p1", 200);
  game.addPlayer(a2, "p2", 300);
  // queue up the actions and play the hand
  // preflop
  a0->queueAction(Action(RAISE, 100));
  a1->queueAction(Action(CALL, 95));
  a2->queueAction(Action(CALL, 90));
  // flop
  a1->queueAction(Action(RAISE, 30));
  a2->queueAction(Action(CALL, 30));
  // turn
  a1->queueAction(Action(RAISE, 50));
  a2->queueAction(Action(CALL, 50));
  // river
  a1->queueAction(Action(CHECK));
  a2->queueAction(Action(CHECK));
  game.play(1);

  const HandHistory& hh = view->getHandHistory();
  ASSERT_TRUE(hh.multipleWinners());
  
  Player winner, p0, p1, p2;
  std::map<size_t, uint32_t> player_winnings = hh.getPlayerWinnings();
  EXPECT_EQ(view->getPlayerBySeat(0, &p0), 0);
  EXPECT_EQ(view->getPlayerBySeat(1, &p1), 0);
  EXPECT_EQ(view->getPlayerBySeat(2, &p2), 0);
  EXPECT_EQ(p0.getChips(), 300);
  EXPECT_EQ(player_winnings.at(0), 300);
  EXPECT_EQ(p1.getChips(), 20);
  EXPECT_EQ(player_winnings.count(1), 0);
  EXPECT_EQ(p2.getChips(), 280);
  EXPECT_EQ(player_winnings.at(2), 160);
}  

// Everyone all in for different amounts
TEST(GameTest, EveryoneAllIn) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();

  std::shared_ptr<TestAgent> a0(new TestAgent);
  std::shared_ptr<TestAgent> a1(new TestAgent);
  std::shared_ptr<TestAgent> a2(new TestAgent);
  std::shared_ptr<TestAgent> a3(new TestAgent);
  std::shared_ptr<TestAgent> a4(new TestAgent);
  std::shared_ptr<TestAgent> a5(new TestAgent);
  std::shared_ptr<TestAgent> a6(new TestAgent);
  std::shared_ptr<TestAgent> a7(new TestAgent);
  std::shared_ptr<TestAgent> a8(new TestAgent);
  
  game.addPlayer(a0, "p0", 100);
  game.addPlayer(a1, "p1", 200);
  game.addPlayer(a2, "p2", 300);
  game.addPlayer(a3, "p3", 400);
  game.addPlayer(a4, "p4", 500);
  game.addPlayer(a5, "p5", 600);
  game.addPlayer(a6, "p6", 700);
  game.addPlayer(a7, "p7", 800);
  game.addPlayer(a8, "p8", 900);

  a3->queueAction(Action(RAISE, 400));
  a4->queueAction(Action(RAISE, 500));
  a5->queueAction(Action(RAISE, 600));
  a6->queueAction(Action(RAISE, 700));
  a7->queueAction(Action(RAISE, 800));
  a8->queueAction(Action(RAISE, 900));
  a0->queueAction(Action(CALL, 100));
  a1->queueAction(Action(CALL, 195));
  a2->queueAction(Action(CALL, 290));

  game.play(1);
  
  const HandHistory &hh = view->getHandHistory();
  std::map<size_t, uint32_t> pwin = hh.getPlayerWinnings();
  std::map<size_t, uint32_t> pchips;
  pchips[0] = 100;
  pchips[1] = 200;
  pchips[2] = 300;
  pchips[3] = 400;
  pchips[4] = 500;
  pchips[5] = 600;
  pchips[6] = 700;
  pchips[7] = 800;
  pchips[8] = 900;

  // simply make sure no player won more than they could have contributed,
  // and that the total winnings sum to the pot - don't wanna
  // reimplement the same alg
  uint32_t pot = 100 + 200 + 300 + 400 + 500 + 600 + 700 + 800 + 900;
  for (auto it = pwin.begin(); it != pwin.end(); ++it) {
    // can win at most what they contribute from each player after them
    // (with more chips), and what each player before them has
    // lil bit of gauss' sum
    ASSERT_TRUE(it->second <= pchips[it->first] * (9 - it->first) +
                100 * (it->first * (it->first + 1) / 2));
    pot -= it->second;
  }
}

// Play a full game between two players choosing random actions,
// one of em should win
TEST(GameTest, TwoRandomAgentFullGame) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();

  std::shared_ptr<RandomAgent> a0(new RandomAgent());
  std::shared_ptr<RandomAgent> a1(new RandomAgent());
  
  game.addPlayer(a0, "p0", 500);
  game.addPlayer(a1, "p1", 500);

  game.play();

  std::map<size_t, Player> players = view->getPlayers();

  EXPECT_EQ(players.size(), 1);
  EXPECT_EQ(players.begin()->second.getChips(), 1000);
}

// 3 players all in, one folds, and two players continue on the next hand.
TEST(GameTest, TwoPlayersRemain) {
  Game game(5, 10);
  std::shared_ptr<const GameView> view = game.getView();

  std::shared_ptr<TestAgent> a0(new TestAgent());
  std::shared_ptr<TestAgent> a1(new TestAgent());
  std::shared_ptr<TestAgent> a2(new TestAgent());
  std::shared_ptr<TestAgent> a3(new TestAgent());
  
  game.addPlayer(a0, "p0", 100);
  game.addPlayer(a1, "p1", 100);
  game.addPlayer(a2, "p2", 100);
  game.addPlayer(a3, "p3", 100);

  a2->queueAction(Action(RAISE, 100));
  a0->queueAction(Action(CALL));
  a1->queueAction(Action(FOLD));
  a3->queueAction(Action(CALL, 90));

  // hand goes to showdown, either a0, a2, or a3 wins
  // doesn't matter who, just queue fold for everyone
  a0->queueAction(Action(FOLD));
  a1->queueAction(Action(FOLD));
  a2->queueAction(Action(FOLD));
  a3->queueAction(Action(FOLD));
}

// chop pot
TEST(GameTest, TwoPlayerAllInChopPot) {
  // set the random seed to 1023914130 to force a chop pot
  // with 7 TightAgents, each starting with 10,000 chips, 10/20 blinds, on
  // hand 1070 bot3 and bot5 (the last two players) should both go all in
  // with 33, and chop the resulting pot. bot3 and bot5 should end up
  // with the same chips they start the hand with, 76700 and 3300 respectively.
  Game game(10, 20);
  auto bot0 = std::make_shared<TightAgent>();
  auto bot1 = std::make_shared<TightAgent>();
  auto bot2 = std::make_shared<TightAgent>();
  auto bot3 = std::make_shared<TightAgent>();
  auto bot4 = std::make_shared<TightAgent>();
  auto bot5 = std::make_shared<TightAgent>();
  auto bot6 = std::make_shared<TightAgent>();
  auto bot7 = std::make_shared<TightAgent>();
  game.addPlayer(bot0, "bot0", 10000);
  game.addPlayer(bot1, "bot1", 10000);
  game.addPlayer(bot2, "bot2", 10000);
  game.addPlayer(bot3, "bot3", 10000);
  game.addPlayer(bot4, "bot4", 10000);
  game.addPlayer(bot5, "bot5", 10000);
  game.addPlayer(bot6, "bot6", 10000);
  game.addPlayer(bot7, "bot7", 10000);

  Random::setSeed(1023914130);
  game.play(1070);

  std::shared_ptr<const GameView> view = game.getView();
  std::map<size_t, Player> players = view->getPlayers();
  EXPECT_EQ(players[3].getChips(), 76700);
  EXPECT_EQ(players[5].getChips(), 3300);

  game.play(1);

  players = view->getPlayers();
  EXPECT_EQ(players[3].getChips(), 76700);
  EXPECT_EQ(players[5].getChips(), 3300);
}




// TODO:
// Three player all in test hand 9, p1 p2 chop with straights, p0 dies?
// seed: 3664792715

// TODO:
// Three player all in test hand 3, p0 p1 (short stacks) chop
// seed: 2702894553


// TODO:
TEST(GameTest, LegalActions) {
  // raise amounts
  // folds, checks
  // no posts
  // calls
}

// TODO:
// Case second player to act is all in, first player still prompted to act
// I think this is fixed now, but need the test case anyway
TEST(GameTest, AllInNoAction) {

}

// TODO:
// ensure update view in right places (was missing after new round)
