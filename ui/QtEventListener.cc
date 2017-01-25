#include <QObject>
#include <QDebug>

#include "QtEventListener.h"

QtEventListener::QtEventListener() {
  QObject(nullptr);
}

void
QtEventListener::onGameStart(const GameView *view) {
  QString s = "game start";
  emit gameStart(s);
}

void
QtEventListener::onGameEnd() {
  emit gameEnd("game end");
}

void
QtEventListener::onPlayerJoin(Player player) {
  QString s = "player join";
  emit playerJoin(s);
}

void
QtEventListener::onPlayerLeave(Player player) {
  QString s = "player leave";
  emit playerLeave(s);
}

void
QtEventListener::onHandStart(long handNum, const GameView *view) {
  QString s = "hand start";
  emit handStart(s);
}

void
QtEventListener::onDeal(STREET street) {
  QString s = "deal";
  emit deal(s);
}

void
QtEventListener::onPlayerAction(Action action) {
  QString s = "player action";
  emit playerAction(s);
}

void
QtEventListener::onShowCards(std::pair<Card, Card> cards,
                                   Player player) {
  QString s = "show cards";
  emit showCards(s);
}

void
QtEventListener::onShowdown(Hand best_hand, Player player) {
  QString s = "showdown";
  emit showdown(s);
}

void
QtEventListener::onPotWin(uint32_t pot, Player player) {
  QString s = "pot win";
  emit potWin(s);
}
