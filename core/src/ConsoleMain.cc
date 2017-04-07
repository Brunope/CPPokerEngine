#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "TightAgent.h"
#include "LoggerEventListener.h"

int main(void) {
  // Create a no limit game with static blinds at 10/20
  Game game(10, 20);

  // Add a listener that logs game events to stdout
  std::shared_ptr<LoggerEventListener> listener(new LoggerEventListener());
  game.addEventListener(listener);

  // Add some players to the game, human vs computer.
  auto human = std::make_shared<HumanAgent>();
  auto bot0 = std::make_shared<TightAgent>();
  auto bot1 = std::make_shared<TightAgent>();
  auto bot2 = std::make_shared<TightAgent>();
  auto bot3 = std::make_shared<TightAgent>();
  auto bot4 = std::make_shared<TightAgent>();
  auto bot5 = std::make_shared<TightAgent>();
  auto bot6 = std::make_shared<TightAgent>();
  auto bot7 = std::make_shared<TightAgent>();
  
  // Each starts with 1000 chips.
  //game.addPlayer(human, "human", 1000);
  game.addPlayer(bot0, "bot0", 10000);
  game.addPlayer(bot1, "bot1", 10000);
  game.addPlayer(bot2, "bot2", 10000);
  game.addPlayer(bot3, "bot3", 10000);
  game.addPlayer(bot4, "bot4", 10000);
  game.addPlayer(bot5, "bot5", 10000);
  game.addPlayer(bot6, "bot6", 10000);
  game.addPlayer(bot7, "bot7", 10000);

  // play until one player wins all the chips
  game.play();
}
