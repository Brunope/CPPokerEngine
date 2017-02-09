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
GameView::getPot() const {
  return pot_;
}

uint32_t
GameView::getCurrentBet() const {
  return current_bet_;
}

uint32_t
GameView::getCurrentRaiseBy() const {
  return current_raise_by_;
}

size_t
GameView::getActingPlayerSeat() const {
  return acting_player_seat_;
}

Player
GameView::getActingPlayer() const {
  return players_.at(acting_player_seat_);
}

const bool *
GameView::getLegalActions() const {
  return legal_actions_;
}

int
GameView::getPlayerBySeat(size_t seat, Player *player) const {
  // todo
  if (players_.count(seat) && player != nullptr) {
    *player = players_.at(seat);
    return 0;
  }
  return 1;
}

int
GameView::getPlayerByName(std::string name, Player *player) const {
  if (player != nullptr) {
    for (auto it = players_.begin(); it != players_.end(); ++it) {
      if (it->second.getName() == name) {
        *player = it->second;
        return 0;
      }
    }
  }
  return 1;
}

Player
GameView::getNextPlayer(const Player &player) const {
  size_t seat = (player.getSeat() + 1) % MAX_NUM_PLAYERS;
  while (seat != player.getSeat()) {
    if (players_.count(seat)) {
      return players_.at(seat);
    }
    seat = (seat + 1) % MAX_NUM_PLAYERS;
  }
  return player;
}

Player
GameView::getNextLivePlayer(const Player &player) const {
  size_t seat = (player.getSeat() + 1) % MAX_NUM_PLAYERS;
  Player current = getNextPlayer(player);
  while (current.getSeat() != player.getSeat()) {
    if (current.isLive()) {
      return current;
    }
    current = getNextPlayer(current);
  }
  return player;
}

std::map<size_t, Player>
GameView::getPlayers() const {
  return players_;
}

std::map<size_t, Player>
GameView::getLivePlayers() const {
  std::map<size_t, Player> live;
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    if (it->second.isLive()) {
      live[it->first] = it->second;
    }
  }
  return live;
}

std::vector<Card>
GameView::getBoard() const {
  return board_;
}

const HandHistory &
GameView::getHandHistory() const {
  return history_;
}

uint32_t
GameView::getHandNum() const {
  return hand_num_;
}
