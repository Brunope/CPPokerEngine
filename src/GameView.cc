#include <cstddef>
#include <vector>
#include <cassert>

#include "GameView.h"

uint32_t
GameView::getBigBlind() const {
  return big_blind_;
}

uint32_t
GameView::getSmallBlind() const {
  return small_blind_;
}

size_t
GameView::getNumPlayers() const {
  return players_.size();
}

size_t
GameView::getButtonPosition() const {
  return button_pos_;
}

STREET
GameView::getStreet() const {
  return street_;
}

uint32_t
GameView::getCurrentBet() const {
  // TODO
  return getBigBlind();  // wrong
}

const Player *
GameView::getPlayerInSeat(size_t seat) const {
  assert(seat < getNumPlayers());
  if (players_.count(seat)) {
    return &players_.at(seat);
  }
  return nullptr;
}

const Player *
GameView::getPlayerByName(std::string name) const {
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    if (it->second.getName() == name) {
      return &(it->second);
    }
  }
  return nullptr;
}

const std::map<size_t, Player> &
GameView::getPlayers() {
  return players_;
}

const std::map<size_t, Player *> &
GameView::getPlayersInHand() {
  return live_players_;
}

const std::vector<Card> &
GameView::getBoard() {
  return board_;
}

const std::vector<Action> *
GameView::getHandAction() {
  return hand_action_;
}

// TODO: getRoundAction
const std::vector<Action> &
GameView::getRoundAction() {
  return hand_action_[street_];
}
