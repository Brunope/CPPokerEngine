#ifndef ACTOR_H_
#define ACTOR_H_

#include <cstdlib>
#include <utility>
#include <vector>

#include "Action.h"
#include "Player.h"
#include "Card.h"

class GameView;

/**
 * Actor is the class that makes decisions in the Game. If you're writing
 * a poker agent, you'll subclass Actor and override its methods.
 */
class Actor {
public:
  // Called once at the beginning of each Hand in which this Actor is
  // participating.
  virtual void receiveHoleCards(const std::pair<Card, Card> hc) = 0;

  // Called every time it's this Actor's turn to act. Return an Action,
  // which contains the type of action and an amount.
  virtual Action act(const GameView &gameView) = 0;

  // Called at the end of every hand. history contains the sequence of
  // Actions that made up the hand, sorted in chronological order, ie
  // most recent Action at the back. 
  virtual void receiveHandHistory(const std::vector<const Action> &history) = 0;

  // Called once by Game when a new Player / Actor is added. Gives the Actor
  // a reference to its own Player object that stores data on stack size,
  // seat position, name, etc.
  void setPlayer(Player *self) { self_ = self; }

protected:
  Player *self_;
  std::pair<Card, Card> hc_;
};
#endif  // ACTOR_H_
