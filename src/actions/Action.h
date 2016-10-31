#ifndef ACTION_H_
#define ACTION_H_

#include "Player.h"

enum ACTION_T { RAISE, CALL, FOLD, CHECK, POST, DEAL, SHOWDOWN,
                NUM_ACTIONS };

/**
 * An Action is some event in the Game. Usually the Game will query an Actor
 * for an Action, and the Actor will construct and return one. But Actions
 * can also represent events in the Game, like the dealing of hole cards or
 * streets.
 */
class Action {
public:
  virtual inline ACTION_T getType() { return NUM_ACTIONS; }
  const Player* getSource() const { return source_; }
protected:
  const Player *source_;

  Action(const Player *source) : source_(source) { }
};

#endif  // ACTION_H_
