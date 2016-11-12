#ifndef I_EVENT_LISTENER_H_
#define I_EVENT_LISTENER_H_

#include <string>

#include "Action.h"
#include "GameView.h"

/**
 * EventListener defines a pure abstract class whose methods will be 
 * called by the Game class after certain state changes in the Game. 
 */
class IEventListener {
public:
  // Called at the same time as Game::play().
  // The Game class retains ownership of the GameView object pointed to
  // by 'game', which at any time contains a snapshot of the Game's state.
  virtual void onGameStart(const GameView *game) = 0;

  // Called when a Player is added to the Game, the rest of the Player's
  // information is found by GameView::getPlayerByName().
  virtual void onPlayerJoin(std::string playerName) = 0;

  // Called when a Player leaves the game (loses all their chips)
  virtual void onPlayerLeave(std::string playerName) = 0;

  // Called at the start of every new hand. 'handNum' starts at 0,
  // increments by 1. 'game' will point to the same object passed
  // in onGameStart(), in case an EventListener is added midway
  // through a game. The Game class retains ownship of 'game'.
  virtual void onHandStart(long handNum, const GameView *game) = 0;
  
  // Called once each for the pre-flop deal (hole cards), flop, turn, and
  // river.
  virtual void onDeal(STREET street) = 0;

  // Called when a Player makes any Action, ie Raise, Fold, Post, or even
  // when they Show their cards at showdown.
  virtual void onPlayerAction(Action action) = 0;

  // Called when two or more players complete the round of betting on
  // the river and compare hands. 'bestHand' is the Hand of the winner of
  // the hand, and 'playerName' is the name of the winning Player.
  virtual void onShowdown(Hand bestHand, std::string playerName) = 0;

  // Called right after onShowdown(). The next Hand will start soon after
  // this.
  virtual void onPotWin(int pot, std::string playerName) = 0;
};
#endif  // I_EVENT_LISTENER_H_
