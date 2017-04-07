#ifndef HANDHISTORY_H_
#define HANDHISTORY_H_

#include <vector>
#include <map>

#include "GameDefs.h"
#include "Action.h"
#include "Player.h"
#include "Hand.h"

class Game;

class HandHistory {
public:
  HandHistory();
  // deep copy hand_action_ of other
  HandHistory(const HandHistory &other);

  // same as copy constructor
  HandHistory& operator=(const HandHistory &other);

  const std::vector<Action> getRoundAction(STREET street) const;
  const std::vector<Action> *getHandAction() const;
  const std::map<size_t, Hand> getKnownHands() const;

  // Return true iff more than one player wins a portion of the pot.
  // This might happen if two players have equal hands, or
  // when one or more players go all in, betting
  // continues after, creating a side pot, and the short stacked
  // all in player ends up winning the main pot. Since a player
  // can only win up to what they bet from each other player,
  // the remaining portion of the pot is given to the player
  // with the next best hand who has contributed to the side pot.
  bool multipleWinners() const;
  
  // Return a map from seat numbers of players to the number of chips
  // the player won in this hand. The map only contains multiple
  // entries when multipleWinners() is true.
  const std::map<size_t, uint32_t> getPlayerWinnings() const;

  // Returns the winner of the hand if multipleWinners() is false.
  // Otherwise has undefined behavior.
  const Player getWinner() const;
  
  friend class Game;
  friend class HandHistoryTest_CopyAssignment_Test;
private:
  std::vector<Action> hand_action_[NUM_STREETS];
  std::map<size_t, Hand> known_hands_;
  std::map<size_t, uint32_t> player_winnings_;
  Player winner_;
};

#endif  // HANDHISTORY_H_
