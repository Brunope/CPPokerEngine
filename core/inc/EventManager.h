#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "Action.h"
#include "Player.h"
#include "Hand.h"
#include "IEventListener.h"

class GameView;

/**
 * EventManager stores a number of event listeners, and updates all
 * listeners on each fired event.
 */
class EventManager {
public:
  // EventManager does NOT take ownership of 'listener'
  void addEventListener(std::shared_ptr<IEventListener> listener);

  // Remove the first occurrence of 'listener'. Any duplicates
  // will remain.
  void removeEventListener(std::shared_ptr<IEventListener> listener);

  void fireViewChangedEvent(std::shared_ptr<const GameView> view);
  void fireGameStartEvent(std::shared_ptr<const GameView> view);
  void fireGameEndEvent();

  // todo: change params to const player &
  void firePlayerJoinEvent(Player player);
  void firePlayerLeaveEvent(Player player);

  void fireHandStartEvent(long handNum, std::shared_ptr<const GameView> view);

  // 'street' is the street just dealt
  void fireDealEvent(STREET street);

  void firePlayerActionEvent(Action action);

  // when a player shows their cards (by going to showdown)
  void fireShowCardsEvent(std::pair<Card, Card> cards, Player player);
  
  // when a player wins a hand at showdown
  void fireShowdownEvent(Hand best_hand, Player player);

  // when a player wins a pot
  void firePotWinEvent(uint32_t pot, Player player);
private:
  std::vector<std::shared_ptr<IEventListener>> eventListeners_;
};
#endif  // EVENT_MANAGER_H_
