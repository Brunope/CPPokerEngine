#ifndef QTEVENTLISTENER_H_
#define QTEVENTLISTENER_H_

#include <QObject>
#include <QString>
#include "IEventListener.h"

/**
 * Once registered in a Game via Game::addEventListener, QtEventListener
 * emits signals as the Game's state changes.
 */
class QtEventListener : public QObject, public IEventListener {
  Q_OBJECT

public:
  QtEventListener();
  void onGameStart(const GameView *view);
  void onPlayerJoin(std::string name);
  void onPlayerLeave(std::string name);
  void onHandStart(long handNum, const GameView *view);
  void onDeal(STREET street);
  void onPlayerAction(Action action);
  void onShowCards(std::pair<Card, Card> cards, std::string name);
  void onShowdown(Hand best_hand, std::string name);
  void onPotWin(int pot, std::string name);

signals:
  void gameStart(const QString &sig_text);
  void playerJoin(const QString &sig_text);
  void playerLeave(const QString &sig_text);
  void handStart(const QString &sig_text);
  void deal(const QString &sig_text);
  void playerAction(const QString &sig_text);
  void showCards(const QString &sig_text);
  void showdown(const QString &sig_text);
  void potWin(const QString &sig_text);

private:
  const GameView *view_;
};

#endif  // QTEVENTLISTENER_H_
