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
  const Player getWinner() const;
  
  friend class Game;
private:
  std::vector<Action> hand_action_[NUM_STREETS];
  std::map<size_t, Hand> known_hands_;
  Player winner_;
};

#endif  // HANDHISTORY_H_
