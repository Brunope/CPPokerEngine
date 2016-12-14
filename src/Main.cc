#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "HumanActor.h"
#include "RandomActor.h"
#include "LoggerEventListener.h"

int main(void) {
  Game game(10, 20);
  HumanActor human;
  RandomActor bot;
  LoggerEventListener listener;
  game.addEventListener(&listener);
  game.addPlayer(&human, "human", 1000);
  game.addPlayer(&bot, "bot", 1000);
  game.play();
}
