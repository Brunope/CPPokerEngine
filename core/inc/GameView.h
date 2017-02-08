#ifndef GAME_VIEW_H_
#define GAME_VIEW_H_

#include <vector>
#include <cinttypes>
#include <cstddef>
#include <string>
#include <map>

#include "Card.h"
#include "Hand.h"
#include "Player.h"
#include "Action.h"
#include "Agent.h"
#include "GameDefs.h"
#include "HandHistory.h"

// Since the Game state changes very very frequently, all methods
// return copies over references/pointers to avoid use-after-free bugs.
// This means you should get values from the view as late as possible
// before use. In a single threaded application (current model), it's
// safe to save values over a duration where the Game is guaranteed
// to be unchanged (ie waiting for action from an Agent). 
class GameView {
public:
  // implicit constructors/destructor fine, all members saved to stack
  uint32_t getBigBlind() const;
  uint32_t getSmallBlind() const;
  size_t getNumPlayers() const;
  size_t getButtonPosition() const;
  STREET getStreet() const;
  uint32_t getPot() const;

  // return the total amount to call
  uint32_t getCurrentBet() const;

  // return the difference between the current bet and the last current
  // bet, ie the amount raised BY in the most recent raise.
  uint32_t getCurrentRaiseBy() const;

  // return a boolean array of size NUM_ACTIONS, the value at index
  // ACTION_T indicates the legality of that action, ie if
  // getLegalActions()[CHECK] is true, a CHECK is legal. This array
  // is dependent on the state of the current agent. For example, a raise
  // may be legal for the current agent, but not for the next agent if
  // they do not have enough chips.
  const bool *getLegalActions() const;
  
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
  // Return hand history of current hand. Cleared as soon as a new hand
  // is started
  const HandHistory& getHandHistory() const;
  uint32_t getHandNum() const;

  // Rely on Game to set all these fields
  friend class Game;
  friend class GameViewTest_Simple_Test;
private:
  std::map<size_t, Player> players_;
  std::map<size_t, Player *> live_players_;
  std::map<size_t, Player *> allin_players_;
  
  std::vector<Card> board_;
  std::vector<Action> hand_action_[NUM_STREETS];
  bool legal_actions_[NUM_ACTIONS];
  HandHistory history_;

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
