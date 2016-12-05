#ifndef GAME_VIEW_H_
#define GAME_VIEW_H_

#include <vector>
#include <cinttypes>
#include <cstddef>
#include <string>
#include <map>

#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Action.h"
#include "Actor.h"

// Bad solution - should keep this in Game but don't want the dependency
enum STREET { PREFLOP, FLOP, TURN, RIVER, NUM_STREETS };

class GameView {
public:
  // implicit constructors/destructor fine, all members saved to stack
  uint32_t getBigBlind() const;
  uint32_t getSmallBlind() const;
  size_t getNumPlayers() const;
  size_t getButtonPosition() const;
  STREET getStreet() const;
  uint32_t getCurrentBet() const;
  // returns null if there is no player in 'seat'
  const Player *getPlayerInSeat(size_t seat) const;
  // returns null if there is no player named 'name'
  const Player *getPlayerByName(std::string name) const;
  const std::map<size_t, Player> &getPlayers() const;
  const std::map<size_t, Player *> &getPlayersInHand();
  const std::vector<Card> &getBoard();
  // return array of vectors of actions for the current hand, one vector
  // per street, with NUM_STREETS vectors in the array
  const std::vector<Action> *getHandAction() const;
  // return the vector of actions for the current street
  const std::vector<Action> &getRoundAction() const;

  // Rely on Game to set all these fields
  friend class Game;
  friend class GameViewTest_Simple_Test;
private:
  std::map<size_t, Actor *> actors_;
  std::map<size_t, Player> players_;
  std::map<size_t, Player *> live_players_;
  std::map<size_t, Player *> allin_players_;
  
  std::vector<Card> board_;
  std::vector<Action> hand_action_[NUM_STREETS];

  size_t button_pos_;
  size_t acting_player_seat_;
  uint32_t big_blind_;
  uint32_t small_blind_;
  STREET street_;
  uint32_t hand_num_;
  uint32_t pot_;
  uint32_t current_bet_;
  uint32_t current_raise_by_;
};

#endif  // GAME_VIEW_H_
