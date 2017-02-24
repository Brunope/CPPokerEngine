#include <vector>
#include <memory>

#include "EventManager.h"

void
EventManager::addEventListener(std::shared_ptr<IEventListener> listener) {
  eventListeners_.push_back(listener);
}

void
EventManager::removeEventListener(std::shared_ptr<IEventListener> listener) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    if (it->get() == listener.get()) {
      eventListeners_.erase(it);
      return;
    }
  }
}

void
EventManager::fireViewChangedEvent(std::shared_ptr<const GameView> view) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onViewChanged(view);
  }
}

void
EventManager::fireGameStartEvent(std::shared_ptr<const GameView> view) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onGameStart(view);
  }
}

void
EventManager::fireGameEndEvent() {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onGameEnd();
  }
}

void
EventManager::firePlayerJoinEvent(Player player) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerJoin(player);
  }
}

void
EventManager::firePlayerLeaveEvent(Player player) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPlayerLeave(player);
  }
}

void
EventManager::fireHandStartEvent(long hand_num, std::shared_ptr<const GameView> view) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onHandStart(hand_num, view);
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
EventManager::fireShowCardsEvent(std::pair<Card, Card> cards, Player player) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onShowCards(cards, player);
  }
}

void
EventManager::fireShowdownEvent(Hand best_hand, Player player) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onShowdown(best_hand, player);
  }
}

void
EventManager::firePotWinEvent(uint32_t pot, Player player) {
  for (auto it = eventListeners_.begin(); it != eventListeners_.end(); ++it) {
    (*it)->onPotWin(pot, player);
  }
}

