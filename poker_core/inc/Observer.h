/**
 * Observer provides a standard observer
 * pattern interface.
 */
#ifndef OBSERVER_H_
#define OBSERVER_H_

class Game;

class Observer {
public:
  // Construct an Observer observing the Game game.
  Observer(const Game &game) : game_(game) {}

  // Called each time the observed Game's externally visible state changes.
  // Usually the state change is a new Actor event, aka Action. The most recent
  // Action can be accessed with Game.getLastAction().
  virtual void notify() = 0;
protected:
  const Game &game_;
};

#endif  // OBSERVER_H_
