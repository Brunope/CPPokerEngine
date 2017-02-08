#ifndef QGAMEDRIVER_H_
#define QGAMEDRIVER_H_

#include <QObject>
#include <thread>
#include "QEventListener.h"
#include "QGameView.h"
#include "Game.h"

/**
 * QtGameDriver controls a Game instance and a QtEventListener. The
 * event listener hooks into the Game and emits signals on Game
 * state changes. When QtGameDriver receives a 'startGame' signal, 
 * it spawns a new thread that plays the Game and emits signals
 * via the QtEventListener. QtGameDriver owns the memory of the
 * QtEventListener but not the memory of the Game.
 */
class QGameDriver : public QObject {
  Q_OBJECT

public:
  // Caller retains ownership of game
  QGameDriver(Game *game);

  // deletes listener_ and terminates game_thread_
  ~QGameDriver();
  
  QGameView *getView() { return &view_; }
  QEventListener *getListener();

public slots:
  // if the game is not already running, begins playing the game
  // in a new thread
  void startGame(const QString &text);
  void updateView(const QGameView &sig_view);

private:
  Game *game_;
  QGameView view_;
  QEventListener *listener_;
  std::thread game_thread_;
  bool game_thread_running_;
};

#endif  // QTGAMEDRIVER_H_
