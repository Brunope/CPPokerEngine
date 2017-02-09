#include <QObject>
#include <QDebug>

#include "QEventListener.h"

QEventListener::QEventListener() {
  QObject(0);
}

void
QEventListener::updateView() {
  updateableView_.copyFrom(view_);
  emit viewChanged(updateableView_);
}

void
QEventListener::onGameStart(const GameView *view) {
  view_ = view;
  QString s = "game start";
  updateView();
  emit gameStart(s);
}

void
QEventListener::onGameEnd() {
  updateView();
  emit gameEnd("game end");
}

void
QEventListener::onPlayerJoin(Player player) {
  QString s = QString::fromStdString(player.getName() + " joined");
  updateView();
  emit playerJoin(s);
}

void
QEventListener::onPlayerLeave(Player player) {
  QString s = QString::fromStdString(player.getName() + " left");
  updateView();
  emit playerLeave(s);
}

void
QEventListener::onHandStart(long handNum, const GameView *view) {
  if (view_ != view) {
    std::cout << "handStart event passed view pointing to different memory" \
              << std::endl;
    view_ = view;
  }
  QString s = QString::fromStdString("hand #" + std::to_string(handNum) +
                                     " started");
  updateView();
  emit handStart(s);
}

void
QEventListener::onDeal(STREET street) {
  QString s = "deal";
  updateView();
  emit deal(s);
}

void
QEventListener::onPlayerAction(Action action) {
  QString s = QString::fromStdString(action.str());
  updateView();
  emit playerAction(s);
}

void
QEventListener::onShowCards(std::pair<Card, Card> cards,
                                   Player player) {
  QString s = QString::fromStdString(player.getName() + " shows " +
                                     cards.first.str() + cards.second.str());
  updateView();
  emit showCards(s);
}

void
QEventListener::onShowdown(Hand best_hand, Player player) {
  QString s = QString::fromStdString(player.getName() + " wins with " +
                                     best_hand.str());
  updateView();
  emit showdown(s);
}

void
QEventListener::onPotWin(uint32_t pot, Player player) {
  QString s = QString::fromStdString(player.getName() + " wins " +
                                     std::to_string(pot));
  updateView();
  emit potWin(s);
}

