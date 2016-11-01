#ifndef GAME_VIEW_H_
#define GAME_VIEW_H_

#include <vector>
#include <cinttypes>
#include <cstddef>

#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Actions.h"

// Bad solution - should keep this in Game but don't want the dependency
enum STREET { PREFLOP, FLOP, TURN, RIVER };

class GameView {
public:
  // implicit constructors/destructor fine, all members saved to stack
  uint32_t getBigBlind() const;
  uint32_t getSmallBlind() const;
  size_t getNumPlayers() const;
  size_t getButtonPosition() const;
  STREET getStreet() const;
  uint32_t getCurrentBet() const;
  const Player &getPlayerInSeat(size_t seat) const;
  const std::vector<Player> &getPlayers() const;
  const std::vector<Player *> &getPlayersInHand() const;
  const std::vector<Card> &getBoard() const;
  const std::vector<Action> &getHandAction() const;
  const std::vector<Action> &getRoundAction() const;

  // Rely on Game to set all these fields
  friend class Game;
  friend class GameViewTest_Simple_Test;
private:
  std::vector<Player> players_;
  std::vector<Player *> live_players_;  // pointers into players_
  std::vector<Card> board_;
  std::vector<Action> hand_action_;

  size_t button_pos_;
  uint32_t big_blind_;
  uint32_t small_blind_;
  STREET street_;
};

#endif  // GAME_VIEW_H_
