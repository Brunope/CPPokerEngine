#include <QObject>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include "Game.h"
#include "QGameDriver.h"
#include "RandomAgent.h"


// play the game
// this call blocks, and should only be called from a new thread
static void startGameInThread(Game *game) {
  game->play();
}


QGameDriver::QGameDriver(Game *game) {
  QObject(0);
  game_ = game;
  listener_ = new QEventListener();
  game_->addEventListener(listener_);
  game_thread_running_ = false;
  
  QObject::connect(listener_, SIGNAL(viewChanged(const QGameView&)),
                   this, SLOT(updateView(const QGameView&)),
                   Qt::BlockingQueuedConnection);
}

QGameDriver::~QGameDriver() {
  delete listener_;
  // detach the thread, so that std::terminate ISN'T called as game_thread_
  // goes out of scope. If terminate were called, ALL threads would exit.
  // The OS will clean up the thread when this process finishes.
  // Not great.
  if (game_thread_running_) {
    game_thread_.detach();
  }
}

void
QGameDriver::startGame(const QString &text) {
  if (!game_thread_running_) {
    game_thread_ = std::thread(startGameInThread, game_);
    game_thread_running_ = true;
  }
}

QEventListener *
QGameDriver::getListener() {
  return listener_;
}

void
QGameDriver::updateView(const QGameView &sig_view) {
  view_.copyFromQ(&sig_view);
}
