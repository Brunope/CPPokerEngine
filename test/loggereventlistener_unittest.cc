#include "gtest/gtest.h"
#include "Action.h"
#include "Player.h"
#include "GameView.h"
#include "LoggerEventListener.h"

TEST(LoggerEventListenerTest, Comprehensive) {
  LoggerEventListener listener;
  GameView g;
  Player p1("p1");
  Player p2("p2");
  Hand h("AhAcAsAdKc");

  // Redirect stdout to buffer
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
  
  listener.onGameStart(&g);
  EXPECT_EQ(buffer.str(), "Starting game\n");
  buffer.str("");
  
  listener.onPlayerJoin(p1.getName());
  EXPECT_EQ(buffer.str(), "p1 joined\n");
  buffer.str("");
  
  listener.onPlayerLeave(p2.getName());
  EXPECT_EQ(buffer.str(), "p2 left\n");
  buffer.str("");
  
  listener.onHandStart(0, &g);
  EXPECT_EQ(buffer.str(), "Starting hand #0\n");
  buffer.str("");
  
  listener.onDeal(PREFLOP);
  EXPECT_EQ(buffer.str(), "Dealing cards\n");
  buffer.str("");
  
  listener.onPlayerAction(Action(RAISE, 10, &p1));
  EXPECT_EQ(buffer.str(), "p1 raised to 10\n");
  buffer.str("");
  
  listener.onShowdown(h, p1.getName());
  EXPECT_EQ(buffer.str(), "p1 wins with AhAcAsAdKc\n");
  buffer.str("");
  
  listener.onPotWin(20, p1.getName());
  EXPECT_EQ(buffer.str(), "p1 wins 20\n");
  buffer.str("");

  // Re-enable stdout
  std::cout.rdbuf(old);
}
