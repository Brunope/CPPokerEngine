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
#include "GameDefs.h"

// Since the Game state changes very very frequently, all methods
// return copies over references/pointers to avoid use-after-free bugs.
// This means you should get values from the view as late as possible
// before use. In a single threaded application (current model), it's
// safe to save values over a duration where the Game is guaranteed
// to be unchanged (ie waiting for action from an Actor). 
class GameView {
public:
  // implicit constructors/destructor fine, all members saved to stack
  uint32_t getBigBlind() const;
  uint32_t getSmallBlind() const;
  size_t getNumPlayers() const;
  size_t getButtonPosition() const;
  STREET getStreet() const;
  uint32_t getCurrentBet() const;
  
  // fills in 'player' and returns 0 if 'player' is non-null and there
  // exists a Player in seat 'seat', 1 otherwise
  int getPlayerBySeat(size_t seat, Player *player) const;
  // fills in 'player' and returns 0 if 'player' is non-null and there
  // exists a Player named 'name'
  int getPlayerByName(std::string name, Player *player) const;
  // returns the player sitting to the left of 'player' (left increases
  // with seat number). Will return 'player' if there is only one Player
  // left.
  Player getNextPlayer(const Player &player) const;
  // Returns the next live player to the left of 'player', or 'player'
  // if there is only one live Player remaining.
  // Unspecified behavior if not called during the middle of a hand.
  Player getNextLivePlayer(const Player &player) const;
  std::map<size_t, Player> getPlayers() const;
  std::map<size_t, Player> getPlayersInHand() const;
  std::vector<Card> getBoard() const;
  // fill in hand_action with vectors of actions for the current hand, one
  // vector per street, must hold NUM_STREETS vectors in the array
  // returns 0 on success, 1 otherwise
  int getHandAction(std::vector<Action> *hand_action) const;
  // return the vector of actions for the current street
  std::vector<Action> getRoundAction() const;

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
  uint32_t small_blind_;
  uint32_t big_blind_;
  STREET street_;
  uint32_t hand_num_;
  uint32_t pot_;
  uint32_t current_bet_;
  uint32_t current_raise_by_;
};

#endif  // GAME_VIEW_H_
