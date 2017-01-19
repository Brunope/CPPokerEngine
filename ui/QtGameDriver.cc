#include <QObject>
#include <cstdlib>
#include <thread>
#include "Game.h"
#include "QtGameDriver.h"


// play the game
// this call blocks, and should only be called from a new thread
static void startGameInThread(Game *game) {
  game->play();
}


QtGameDriver::QtGameDriver(Game *game) {
  QObject(0);
  game_ = game;
  listener_ = new QtEventListener();
  game_->addEventListener(listener_);
  game_thread_running_ = false;
}

QtGameDriver::~QtGameDriver() {
  delete listener_;
  // game_thread_ is terminated as it goes out of scope
}

void
QtGameDriver::startGame(const QString &text) {
  if (!game_thread_running_) {
    game_thread_ = std::thread(startGameInThread, game_);
    game_thread_running_ = true;
  }
}

QtEventListener *
QtGameDriver::getListener() {
  return listener_;
}
