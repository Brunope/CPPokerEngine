#include <vector>

#include "EventManager.h"

void
EventManager::addEventListener(IEventListener *listener) {
  eventListeners_.push_back(listener);
}

void
EventManager::removeEventListener(IEventListener *listener) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    if ((*it) == listener) {
      eventListeners_.erase(it);
      // important: if we don't break here, we'll incorrectly increment the
      // iterator and either miss an element, or seg fault
      // if we have to keep going after erasing, decrement 'it' here.
      break;
    }
  }
}

void
EventManager::fireGameStartEvent(const GameView &game) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onGameStart(&game);
  }
}

void
EventManager::fireGameOverEvent(const GameView &game) {

}

void
EventManager::firePlayerJoinEvent(std::string playerName) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerJoin(playerName);
  }
}

void
EventManager::firePlayerLeaveEvent(std::string playerName) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerLeave(playerName);
  }
}

void
EventManager::fireHandStartEvent(long handNum, const GameView &game) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onHandStart(handNum, &game);
  }
}

void
EventManager::fireDealEvent(STREET street) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onDeal(street);
  }
}

void
EventManager::firePlayerActionEvent(Action action) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerAction(action);
  }
}

void
EventManager::fireShowdownEvent(Hand bestHand, std::string playerName) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onShowdown(bestHand, playerName);
  }
}

void
EventManager::firePotWinEvent(int pot, std::string playerName) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPotWin(pot, playerName);
  }
}

