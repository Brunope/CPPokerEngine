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
EventManager::fireGameStartEvent(const GameView &view) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onGameStart(&view);
  }
}

void
EventManager::fireGameOverEvent(const GameView &view) {

}

void
EventManager::firePlayerJoinEvent(std::string name) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerJoin(name);
  }
}

void
EventManager::firePlayerLeaveEvent(std::string name) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerLeave(name);
  }
}

void
EventManager::fireHandStartEvent(long hand_num, const GameView &view) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onHandStart(hand_num, &view);
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
EventManager::fireShowCardsEvent(std::pair<Card, Card> cards,
                                 std::string name) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onShowCards(cards, name);
  }
}

void
EventManager::fireShowdownEvent(Hand best_hand, std::string name) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onShowdown(best_hand, name);
  }
}

void
EventManager::firePotWinEvent(int pot, std::string name) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPotWin(pot, name);
  }
}

