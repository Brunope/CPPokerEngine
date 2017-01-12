#include <cassert>

#include "HandHistory.h"

HandHistory::HandHistory() {
}

HandHistory::HandHistory(const HandHistory &other) {
  // wish we could iterate over an enum
  hand_action_[PREFLOP] = other.hand_action_[PREFLOP];
  hand_action_[FLOP] = other.hand_action_[FLOP];
  hand_action_[TURN] = other.hand_action_[TURN];
  hand_action_[RIVER] = other.hand_action_[RIVER];
  known_hands_ = other.known_hands_;
  player_winnings_ = other.player_winnings_;
  winner_ = other.winner_;
}

HandHistory &
HandHistory::operator=(const HandHistory &other) {
  if (&other == this) {
    return *this;
  }
  hand_action_[PREFLOP] = other.hand_action_[PREFLOP];
  hand_action_[FLOP] = other.hand_action_[FLOP];
  hand_action_[TURN] = other.hand_action_[TURN];
  hand_action_[RIVER] = other.hand_action_[RIVER];
  known_hands_ = other.known_hands_;
  player_winnings_ = other.player_winnings_;
  winner_ = other.winner_;
  return *this;
}

const std::vector<Action>
HandHistory::getRoundAction(STREET street) const {
  assert(street < NUM_STREETS);
  return hand_action_[street];
}

const std::vector<Action> *
HandHistory::getHandAction() const {
  return hand_action_;
}

const std::map<size_t, Hand>
HandHistory::getKnownHands() const {
  return known_hands_;
}

bool
HandHistory::multipleWinners() const {
  return player_winnings_.size() > 1;
}

const std::map<size_t, uint32_t>
HandHistory::getPlayerWinnings() const {
  return player_winnings_;
}

const Player
HandHistory::getWinner() const {
  return winner_;
}
