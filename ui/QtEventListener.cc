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
QtEventListener::onPlayerJoin(std::string name) {
  QString s = "player join";
  emit playerJoin(s);
}

void
QtEventListener::onPlayerLeave(std::string name) {
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
                                   std::string name) {
  QString s = "show cards";
  emit showCards(s);
}

void
QtEventListener::onShowdown(Hand best_hand, std::string name) {
  QString s = "showdown";
  emit showdown(s);
}

void
QtEventListener::onPotWin(int pot, std::string name) {
  QString s = "pot win";
  emit potWin(s);
}
