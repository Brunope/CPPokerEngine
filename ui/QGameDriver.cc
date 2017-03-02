#include <QObject>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include "Game.h"
#include "QGameDriver.h"
#include "RandomAgent.h"


// play the game
// this call blocks, and should only be called from a new thread
static void startGameInThread(std::shared_ptr<Game> game) {
  game->play();
}


QGameDriver::QGameDriver(std::shared_ptr<Game> game) {
  QObject(0);
  game_ = game;
  listener_ = std::make_shared<QEventListener>();
  game_->addEventListener(listener_);
  game_thread_running_ = false;
  
  QObject::connect(listener_.get(), SIGNAL(viewChanged(const QGameView&)),
                   this, SLOT(updateView(const QGameView&)),
                   Qt::BlockingQueuedConnection);
}

QGameDriver::~QGameDriver() {
  std::cout << "destructing qgamedriver" << std::endl;
  // detach the thread, so that std::terminate ISN'T called as game_thread_
  // goes out of scope. If terminate were called, ALL threads would exit.
  // The OS will clean up the thread when this process finishes.
  // Not great.
  if (game_thread_running_) {
    std::cout << "detaching game thread" << std::endl;
    game_thread_.detach();
  }
  std::cout << "destructed qgamedriver" << std::endl;
}

void
QGameDriver::startGame(const QString &text) {
  if (!game_thread_running_) {
    game_thread_ = std::thread(startGameInThread, game_);
    game_thread_running_ = true;
  }
}

std::shared_ptr<QEventListener>
QGameDriver::getListener() {
  return listener_;
}

void
QGameDriver::updateView(const QGameView &sig_view) {
  view_.copyFromQ(&sig_view);
}
