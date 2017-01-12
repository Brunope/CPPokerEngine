#include <sstream>
#include <cstdio>

#include "Action.h"
#include "GameView.h"
#include "IEventListener.h"
#include "LoggerEventListener.h"

LoggerEventListener::LoggerEventListener() {
}

void
LoggerEventListener::onGameStart(const GameView *view) {
  view_ = view;
  std::cout << "Starting game" << std::endl;
}

void
LoggerEventListener::onPlayerJoin(std::string name) {
  std::cout << name << " joined" << std::endl;
}

void
LoggerEventListener::onPlayerLeave(std::string name) {
  std::cout << name << " left" << std::endl;
}

void
LoggerEventListener::onHandStart(long hand_num, const GameView *view) {
  std::cout << std::endl;
  std::cout << "Starting hand #" << hand_num << std::endl;
  view_ = view;  // update just in case
  std::map<size_t, Player> players = view->getPlayers();
  for (auto it = players.begin(); it != players.end(); ++it) {
    std::cout << "Seat " << it->second.getSeat() << ": " \
              << it->second.getName() << ", " << it->second.getChips() \
              << std::endl;
  }
}

void
LoggerEventListener::onDeal(STREET street) {
  std::cout << "Dealing cards" << std::endl;
  if (street > PREFLOP) {
    std::cout << "board: ";
    std::vector<Card> board = view_->getBoard();
    for (auto it = board.begin(); it != board.end(); ++it) {
      std:: cout << *it;
    }
  std::cout << std::endl;
  }
}

void
LoggerEventListener::onPlayerAction(Action action) {
  std::cout << action << std::endl;
  std::cout << "pot: " << view_->getPot() << std::endl;
}

void
LoggerEventListener::onShowCards(std::pair<Card, Card> cards,
                                 std::string name) {
  std::cout << name << " shows " << cards.first << cards.second << std::endl;
}

void
LoggerEventListener::onShowdown(Hand best_hand, std::string name) {
  std::cout << name << " wins with " << best_hand.str() << std::endl;
}

void
LoggerEventListener::onPotWin(int pot, std::string name) {
  std::cout << name << " wins " << pot << std::endl;
}
