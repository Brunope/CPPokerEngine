#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <vector>
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

  // Remove the first occurrence of 'listener'. Any added duplicates
  // will remain.
  void removeEventListener(IEventListener *listener);
  
  void fireGameStartEvent(const GameView *game);
  void firePlayerJoinEvent(std::string playerName);
  void firePlayerLeaveEvent(std::string playerName);
  void fireHandStartEvent(long handNum, const GameView *game);
  void fireDealEvent(STREET street);
  void firePlayerActionEvent(Action action);
  void fireShowdownEvent(Hand bestHand, std::string playerName);
  void firePotWinEvent(int pot, std::string playerName);
private:
  std::vector<IEventListener *> eventListeners_;
};
#endif  // EVENT_MANAGER_H_
