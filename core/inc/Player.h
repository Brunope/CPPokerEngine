#ifndef PLAYER_H_
#define PLAYER_H_

#include <cstdint>
#include <string>

/**
 * Player stores all the information associated with a participant in the Game.
 * It is immutable, because any Agent should be able to view any
 * Player in the Game, but should not be able to mutate any of the information
 * (Game takes sole responsibility).
 */
class Player {
public:
  Player(std::string name = "") : name_(name), live_(false) { }

  inline std::string getName() const { return name_; }
  inline uint32_t getChips() const { return chips_; }
  inline uint32_t getChipsInPlay() const { return chips_in_play_; }
  inline uint32_t getSeat() const { return seat_; }

  // A Player is live if they have not folded in the current hand
  inline bool isLive() const { return live_; }
  
  // for this to be correct, rely on Game invariant of removing Players
  // that have lost all their chips as soon as possible.
  inline bool isAllIn() const { return chips_ == 0; }

  // Game modifies the chips. Should be the only friend (sad)
  friend class Game;
private:
  std::string name_;
  uint32_t chips_;
  uint32_t chips_in_play_;
  size_t seat_;
  bool live_;
};

#endif  // PLAYER_H_
