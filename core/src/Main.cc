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
  LoggerEventListener listener;
  game.addEventListener(&listener);

  // Add some players to the game, human vs computer.
  // Each starts with 1000 chips.
  HumanAgent human;
  RandomAgent bot0, bot1, bot2;
  game.addPlayer(&human, "human", 1000);
  game.addPlayer(&bot0, "bot0", 1000);
  game.addPlayer(&bot1, "bot1", 1000);
  // game.addPlayer(&bot2, "bot2", 1000);

  // play until one player wins all the chips
  game.play();
}
