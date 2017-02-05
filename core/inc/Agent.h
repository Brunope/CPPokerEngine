#ifndef AGENT_H_
#define AGENT_H_

#include <cstdlib>
#include <utility>
#include <vector>

#include "Action.h"
#include "Player.h"
#include "HandHistory.h"
#include "Card.h"

class GameView;

/**
 * Agent is the class that makes decisions in the Game. If you're writing
 * a poker agent, you'll subclass Agent and override its methods.
 */
class Agent {
public:
  // Called once at the beginning of each Hand in which this Agent is
  // participating.
  virtual void receiveHoleCards(const std::pair<Card, Card> hc) = 0;

  // Called every time it's this Agent's turn to act. Return an Action,
  // which contains the type of action and an amount.
  virtual Action act(const GameView &view) = 0;

  // Called at the end of every hand. history contains the sequence of
  // Actions that made up the hand, sorted in chronological order, ie
  // most recent Action at the back. 
  virtual void receiveHandHistory(const HandHistory &history) = 0;

  // Called once by Game when this new Player / Agent is added. Gives the Agent
  // a reference to its own Player object that stores data on stack size,
  // seat position, and name.
  void setPlayer(const Player *self) { self_ = self; }

protected:
  const Player *self_;
  std::pair<Card, Card> hc_;
};
#endif  // AGENT_H_
