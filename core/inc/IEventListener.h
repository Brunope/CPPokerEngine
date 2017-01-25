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
  // The view is guaranteed to contain a snapshot of the Game's state
  // at any time before the call to onGameEnd() returns. The Game retains
  // ownership of the view's memory. 
  virtual void onGameStart(const GameView *view) = 0;

  // Called when the Game ends, either because the maximum number of hands
  // have been played, or one player has collected all the chips. The GameView
  // pointer from onGameStart can still be used throughout this method, but
  // not after. The only call that can follow this method is onGameStart().
  virtual void onGameEnd() = 0;

  // Called when a Player is added to the Game, the rest of the Player's
  // information is found by GameView::getPlayerByName().
  virtual void onPlayerJoin(Player player) = 0;

  // Called when a Player leaves the game (loses all their chips)
  virtual void onPlayerLeave(Player player) = 0;

  // Called at the start of every new hand. 'handNum' starts at 0,
  // increments by 1. The view will point to the same object passed
  // in onGameStart(), in case an EventListener is added midway
  // through a game. The Game class retains ownship of the view's memory.
  virtual void onHandStart(long hand_num, const GameView *view) = 0;
  
  // Called once each for the pre-flop deal (hole cards), flop, turn, and
  // river. 'street' is the street just dealt. 
  virtual void onDeal(STREET street) = 0;

  // Called when a Player makes any Action, ie Raise, Fold, Post
  virtual void onPlayerAction(Action action) = 0;

  // Called when a Player shows their hole cards by going to showdown.
  // Called once for each Player.
  virtual void onShowCards(std::pair<Card, Card> cards, Player player) = 0;

  // Called when two or more players complete the round of betting on
  // the river and compare hands. 'name' is the name of the Player
  // with best 5-card hand. May be called more than once per hand
  // in the case of many all ins and side pots.
  virtual void onShowdown(Hand best_hand, Player player) = 0;

  // Called as soon as the winner of the hand is decided. In the case of
  // all ins and side pots, there may be multiple winners per hand.
  virtual void onPotWin(uint32_t pot, Player player) = 0;
};
#endif  // I_EVENT_LISTENER_H_
