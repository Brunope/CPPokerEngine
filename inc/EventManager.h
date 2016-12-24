#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <vector>
#include <utility>
#include <string>

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
  void addEventListener(IEventListener *listener);

  // Remove the first occurrence of 'listener'. Any duplicates
  // will remain.
  void removeEventListener(IEventListener *listener);
  
  void fireGameStartEvent(const GameView &view);
  void fireGameOverEvent(const GameView &view);

  // todo: change params to const player &
  void firePlayerJoinEvent(std::string name);
  void firePlayerLeaveEvent(std::string name);

  void fireHandStartEvent(long handNum, const GameView &view);

  // 'street' is the street just dealt
  void fireDealEvent(STREET street);

  void firePlayerActionEvent(Action action);

  // when a player shows their cards (by going to showdown)
  void fireShowCardsEvent(std::pair<Card, Card> cards, std::string name);
  
  // when a player wins a hand at showdown
  void fireShowdownEvent(Hand best_hand, std::string name);

  // when a player wins a pot
  void firePotWinEvent(int pot, std::string name);
private:
  std::vector<IEventListener *> eventListeners_;
};
#endif  // EVENT_MANAGER_H_
