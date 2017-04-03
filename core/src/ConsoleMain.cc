#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "LoggerEventListener.h"

int main(void) {
  // Create a no limit game with static blinds at 10/20
  Game game(10, 20);

  // Add a listener that logs game events to stdout
  std::shared_ptr<LoggerEventListener> listener(new LoggerEventListener());
  game.addEventListener(listener);

  // Add some players to the game, human vs computer.
  std::shared_ptr<HumanAgent> human(new HumanAgent());
  std::shared_ptr<RandomAgent> bot0(new RandomAgent());
  std::shared_ptr<RandomAgent> bot1(new RandomAgent());
  std::shared_ptr<RandomAgent> bot2(new RandomAgent());
  
  // Each starts with 1000 chips.
  game.addPlayer(human, "human", 1000);
  game.addPlayer(bot0, "bot0", 10000);
  game.addPlayer(bot1, "bot1", 10000);
  game.addPlayer(bot2, "bot2", 10000);

  // play until one player wins all the chips
  game.play();
}
