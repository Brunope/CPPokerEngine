#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "HumanActor.h"
#include "SimpleActor.h"
#include "Observer.h"

int main(void) {
  Game g;

  HumanActor human;
  SimpleActor bot;
  g.addPlayer(human, "human", 1000);
  g.addPlayer(bot, "bot", 1000);

  Observer observer;
  g.addObserver(observer);
  
  g.play();
}
