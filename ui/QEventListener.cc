#include <QObject>
#include <QDebug>
#include <iostream>

#include "QEventListener.h"

QEventListener::QEventListener() {
  QObject(0);
}

QEventListener::~QEventListener() {
  std::cout << "destructed qeventlistener" << std::endl;
}

void
QEventListener::updateView() {
  updateableView_.copyFrom(view_);
  emit viewChanged(updateableView_);
}

void
QEventListener::onViewChanged(std::shared_ptr<const GameView> view) {
  view_ = view;
  updateView();
}

void
QEventListener::onGameStart(std::shared_ptr<const GameView> view) {
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
QEventListener::onHandStart(long handNum, std::shared_ptr<const GameView> view) {
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
  UNUSED(street);
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
  int seat = player.getSeat();
  shown_cards_[seat * 2].copyFrom(cards.first);
  shown_cards_[seat * 2 + 1].copyFrom(cards.second);
  emit showCards(s, updateableView_.player(seat),
                 shown_cards_ + seat * 2, shown_cards_ + seat * 2 + 1);
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
  emit potWin(s, updateableView_.player(player.getSeat()), pot);
}

