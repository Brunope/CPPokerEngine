#include <cassert>
#include <iostream>
#include "GameView.h"
#include "QGameView.h"

QGameView::QGameView() {
  QObject(0);
  initQPlayers();
  num_hands_ = 0;
}

QGameView::~QGameView() {
  for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
    delete players_[i];
  }
}

// todo: incremental updates over mass clobbering everything
void
QGameView::copyFrom(const GameView *view) {
  // copy players, reset them all first, eugh
  for (size_t i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.at(i)->copyFrom(QPlayer());
  }
  std::map<size_t, Player> other_players = view->getPlayers();
  for (auto it = other_players.begin(); it != other_players.end(); ++it) {
    players_.at(it->first)->copyFrom(it->second);
  }
  emit playersChanged();

  num_hands_ = view->getHandNum();
  emit numHandsChanged();
}

void
QGameView::copyFromQ(const QGameView *other) {
  for (size_t i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.at(i)->copyFrom(*(other->players_.at(i)));
  }
  emit playersChanged();

  num_hands_ = other->num_hands_;
  emit numHandsChanged();
}
  

QQmlListProperty<QPlayer>
QGameView::players() {
  return QQmlListProperty<QPlayer>(this, players_);
}

QPlayer *
QGameView::player(int index) const {
  return players_.at(index);
}

int
QGameView::playerCount() const {
  return players_.count();
}

void
QGameView::setPlayer(size_t seat, QPlayer player) {
  assert(seat < MAX_NUM_PLAYERS);
  // if (*(players_[seat]) != player) {
  //   *(players_[seat]) = player;
  //   emit playersChanged();
  // }
  // TODO: provide an operator== for QPlayer (dunno why it cant use the default)
  QPlayer *newPlayer = new QPlayer();
  newPlayer->copyFrom(player);
  players_[seat] = newPlayer;
  emit playersChanged();
}

int
QGameView::numHands() const {
  return num_hands_;
}

void
QGameView::initQPlayers() {
  for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.append(new QPlayer());
  }
}
