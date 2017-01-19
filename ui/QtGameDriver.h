#ifndef QTGAMEDRIVER_H_
#define QTGAMEDRIVER_H_

#include <QObject>
#include <thread>
#include "QtEventListener.h"
#include "Game.h"

/**
 * QtGameDriver controls a Game instance and a QtEventListener. The
 * event listener hooks into the Game and emits signals on Game
 * state changes. When QtGameDriver receives a 'startGame' signal, 
 * it spawns a new thread that plays the Game and emits signals
 * via the QtEventListener. QtGameDriver owns the memory of the
 * QtEventListener but not the memory of the Game.
 */
class QtGameDriver : public QObject {
  Q_OBJECT

public:
  // Caller retains ownership of game
  QtGameDriver(Game *game);

  // deletes listener_ and terminates game_thread_
  ~QtGameDriver();

  QtEventListener *getListener();

public slots:
  // if the game is not already running, begins playing the game
  // in a new thread
  void startGame(const QString &text);

private:
  Game *game_;
  QtEventListener *listener_;
  std::thread game_thread_;
  bool game_thread_running_;
};

#endif  // QTGAMEDRIVER_H_
