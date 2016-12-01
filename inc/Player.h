#ifndef PLAYER_H_
#define PLAYER_H_

#include "Card.h"

/**
 * Player stores all the information associated with a participant in the Game.
 * It is immutable, because any Actor should be able to view any
 * Player in the Game, but should not be able to mutate any of the information
 * (Game takes sole responsibility).
 */
class Player {
public:
  Player(std::string name = "") : name_(name) { }

  inline std::string getName() const { return name_; }
  inline size_t getChips() const { return chips_; }
  inline size_t getChipsInPlay() const { return chips_in_play_; }
  inline size_t getSeat() const { return seat_; }
  
  // for this to be correct, rely on Game invariant of removing Players
  // that have lost all their chips as soon as possible.
  inline bool isAllIn() const { return chips_ == 0; }

  // Game modifies the chips, hole cards, etc. Shuld be the only friend (sad)
  friend class Game;
private:
  std::string name_;
  uint32_t chips_;
  uint32_t chips_in_play_;
  size_t seat_;

  std::pair<Card, Card> hc_;
};

#endif  // PLAYER_H_
