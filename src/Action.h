#ifndef ACTION_H_
#define ACTION_H_

#include <cinttypes>

enum ACTION_T { RAISE, CALL, FOLD, CHECK, POST, NUM_ACTIONS };

class Player;

/**
 * An Action is an Actor supplied action. If the type of an Action is
 * FOLD or CHECK, its amount will always be ignored.
 */
class Action {
public:
  Action(ACTION_T type, uint32_t amount = 0, Player *source = nullptr);
  
  ACTION_T getType() const;
  const Player *getSource() const;
  uint32_t getAmount() const;
private:
  ACTION_T type_;
  uint32_t amount_;
  const Player *source_;

  void checkRep();
};

#endif  // ACTION_H_
