#ifndef ACTION_H_
#define ACTION_H_

#include <cinttypes>
#include <iostream>
#include <string>

#include "Player.h"

enum ACTION_T : uint8_t { RAISE, CALL, FOLD, CHECK, POST, NUM_ACTIONS };


/**
 * An Action is an Agent supplied action. If the type of an Action is
 * FOLD or CHECK, its amount will always be ignored.
 */
class Action {
public:
  Action(ACTION_T type = NUM_ACTIONS, uint32_t amount = 0,
         Player *source = nullptr);
  
  ACTION_T getType() const;
  const Player *getSource() const;
  uint32_t getAmount() const;

  // Useful for debugging - of the form 'NAME ACTIONTYPEs AMOUNT'
  // Requires non-null source and type < NUM_ACTIONS
  std::string str() const;
  friend std::ostream &operator<<(std::ostream &os, const Action &action);
private:
  ACTION_T type_;
  uint32_t amount_;
  const Player *source_;

  void checkRep();
};

#endif  // ACTION_H_
