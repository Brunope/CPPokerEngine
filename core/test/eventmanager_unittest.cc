#include <string>
#include <iostream>
#include <sstream>
#include <memory>

#include "LoggerEventListener.h"
#include "GameView.h"
#include "Player.h"
#include "Hand.h"
#include "EventManager.h"

#include "gtest/gtest.h"

// Rather use a generic listener specifically for testing
TEST(EventManagerTest, LoggerEventListenerIntegration) {
  EventManager e;
  std::shared_ptr<LoggerEventListener> logger(new LoggerEventListener());
  std::shared_ptr<const GameView> g(new const GameView());
  Player p1("p1");
  Player p2("p2");
  Hand h("AhAcAsAdKc");

  e.addEventListener(logger);

  // Redirect stdout to buffer
  std::cout.flush();
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
  
  e.fireGameStartEvent(g);
  EXPECT_EQ(buffer.str(), "Starting game\n");
  buffer.str("");  // clear buffer
  
  e.firePlayerJoinEvent(p1);
  EXPECT_EQ(buffer.str(), "p1 joined\n");
  buffer.str("");
  
  e.firePlayerLeaveEvent(p2);
  EXPECT_EQ(buffer.str(), "p2 left\n");
  buffer.str("");

  // add the same listener again and expect double the output
  e.addEventListener(logger);
  
  e.fireHandStartEvent(0, g);
  EXPECT_EQ(buffer.str(), "\nStarting hand #0\n\nStarting hand #0\n");
  buffer.str("");
  
  e.fireDealEvent(PREFLOP);
  EXPECT_EQ(buffer.str(), "Dealing cards\nDealing cards\n");
  buffer.str("");

  // remove one of them and go back to normal
  e.removeEventListener(logger);
  
  e.firePlayerActionEvent(Action(RAISE, 10, &p1));
  EXPECT_EQ(buffer.str().substr(0, 17), "p1 raises 10\npot:");
  buffer.str("");
  
  e.fireShowdownEvent(h, p1);
  EXPECT_EQ(buffer.str(), "p1 wins with AhAcAsAdKc\n");
  buffer.str("");

  e.firePotWinEvent(20, p1);
  EXPECT_EQ(buffer.str(), "p1 wins 20\n");
  buffer.str("");

  // remove the last one and expect no output
  e.removeEventListener(logger);
  
  e.fireGameStartEvent(g);
  EXPECT_EQ(buffer.str(), "");

  // Re-enable stdout
  std::cout.rdbuf(old);
}  
