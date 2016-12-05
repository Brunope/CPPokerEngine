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
  return current_bet_;
}

int
GameView::getPlayerInSeat(size_t seat, Player *player) const {
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

std::map<size_t, Player>
GameView::getPlayers() const {
  return players_;
}

std::map<size_t, Player>
GameView::getPlayersInHand() const {
  // dereference our live_players_ for em
  std::map<size_t, Player> live;
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    live[it->first] = *(it->second);
  }
  return live;
}

std::vector<Card>
GameView::getBoard() const {
  return board_;
}

int
GameView::getHandAction(std::vector<Action> *hand_action) const {
  if (hand_action != nullptr) {
    hand_action[PREFLOP] = hand_action_[PREFLOP];
    hand_action[FLOP] = hand_action_[FLOP];
    hand_action[TURN] = hand_action_[TURN];
    hand_action[RIVER] = hand_action_[RIVER];
    return 0;
  }
  return 1;
}

std::vector<Action>
GameView::getRoundAction() const {
  return hand_action_[street_];
}
