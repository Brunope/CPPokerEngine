#include <cassert>
#include <iostream>
#include "GameView.h"
#include "QGameView.h"

QGameView::QGameView() {
  QObject(0);
  initQPlayers();
  num_hands_ = 0;
  acting_player_seat_ = -1;
  pot_ = 0;
}

QGameView::~QGameView() {
  for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
    delete players_[i];
  }
  std::cout << "destructed qgameview"  << std::endl;
}

// todo: incremental updates over mass clobbering everything
void
QGameView::copyFrom(std::shared_ptr<const GameView> other) {
  // copy players, reset them all first, eugh
  // TODO: combine these into 1 loop with map::find
  for (size_t i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.at(i)->unexist();
  }
  std::map<size_t, Player> other_players = other->getPlayers();
  for (auto it = other_players.begin(); it != other_players.end(); ++it) {
    players_.at(it->first)->copyFrom(it->second);
  }
  emit playersChanged();

  board_.clear();
  std::vector<Card> other_board = other->getBoard();
  for (size_t i = 0; i < other_board.size(); i++) {
    card_mem_[i].copyFrom(other_board[i]);
    board_.append(&card_mem_[i]);
  }
  emit boardChanged();
  num_hands_ = other->getHandNum();
  emit numHandsChanged();
  acting_player_seat_ = other->getActingPlayerSeat();
  emit actingPlayerSeatChanged();
  pot_ = other->getPot();
  emit potChanged();
  current_bet_ = other->getCurrentBet();
  emit currentBetChanged();
  current_raise_by_ = other->getCurrentRaiseBy();
  emit currentRaiseByChanged();
}

void
QGameView::copyFromQ(const QGameView *other) {
  for (size_t i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.at(i)->copyFrom(*(other->players_.at(i)));
  }
  emit playersChanged();

  board_.clear();
  for (size_t i = 0; i < other->board_.size(); i++) {
    card_mem_[i].copyFromQ(*(other->board_[i]));
    board_.append(&card_mem_[i]);
  }
  emit boardChanged();

  num_hands_ = other->num_hands_;
  emit numHandsChanged();

  acting_player_seat_ = other->acting_player_seat_;
  emit actingPlayerSeatChanged();

  pot_ = other->pot_;
  emit potChanged();
  current_bet_ = other->current_bet_;
  emit currentBetChanged();
  current_raise_by_ = other->current_raise_by_;
  emit currentRaiseByChanged();
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
  QPlayer *newPlayer = new QPlayer();
  newPlayer->copyFrom(player);
  delete players_[seat];
  players_[seat] = newPlayer;
  emit playersChanged();
}

QQmlListProperty<QCard>
QGameView::board() {
  return QQmlListProperty<QCard>(this, board_);
}

QCard *
QGameView::card(int index) const {
  return board_.at(index);
}

int
QGameView::cardCount() const {
  return board_.count();
}

int
QGameView::numHands() const {
  return num_hands_;
}

int
QGameView::actingPlayerSeat() const {
  return acting_player_seat_;
}

int
QGameView::pot() const {
  return pot_;
}

int
QGameView::currentBet() const {
  return current_bet_;
}

int
QGameView::currentRaiseBy() const {
  return current_raise_by_;
}

void
QGameView::initQPlayers() {
  for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
    players_.append(new QPlayer());
  }
}
