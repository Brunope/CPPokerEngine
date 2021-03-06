#include "gtest/gtest.h"
#include "Action.h"
#include "Player.h"
#include "GameView.h"
#include "LoggerEventListener.h"

TEST(LoggerEventListenerTest, Comprehensive) {
  LoggerEventListener listener;
  std::shared_ptr<const GameView> g(new const GameView());
  Player p1("p1");
  Player p2("p2");
  Hand h("AhAcAsAdKc");

  // Redirect stdout to buffer
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
  
  listener.onGameStart(g);
  EXPECT_EQ(buffer.str(), "Starting game\n");
  buffer.str("");  // clear buffer
  
  listener.onPlayerJoin(p1);
  EXPECT_EQ(buffer.str(), "p1 joined\n");
  buffer.str("");
  
  listener.onPlayerLeave(p2);
  EXPECT_EQ(buffer.str(), "p2 left\n");
  buffer.str("");
  
  listener.onHandStart(0, g);
  EXPECT_EQ(buffer.str(), "\nStarting hand #0\n");
  buffer.str("");
  
  listener.onDeal(PREFLOP);
  EXPECT_EQ(buffer.str(), "Dealing cards\n");
  buffer.str("");
  
  listener.onPlayerAction(Action(RAISE, 10, &p1));
  // expect print out action, then pot size, but pot is
  // garbo
  EXPECT_EQ(buffer.str().substr(0, 17), "p1 raises 10\npot:");
  buffer.str("");
  
  listener.onShowdown(h, p1);
  EXPECT_EQ(buffer.str(), "p1 wins with AhAcAsAdKc\n");
  buffer.str("");
  
  listener.onPotWin(20, p1);
  EXPECT_EQ(buffer.str(), "p1 wins 20\n");
  buffer.str("");

  // Re-enable stdout
  std::cout.rdbuf(old);
}
