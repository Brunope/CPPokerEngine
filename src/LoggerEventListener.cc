#include <sstream>
#include <cstdio>

#include "Action.h"
#include "GameView.h"
#include "IEventListener.h"
#include "LoggerEventListener.h"

LoggerEventListener::LoggerEventListener() {
}

void
LoggerEventListener::onGameStart(const GameView *game) {
  game_ = game;
  std::cout << "Starting game" << std::endl;
}

void
LoggerEventListener::onPlayerJoin(std::string playerName) {
  std::cout << playerName << " joined" << std::endl;
}

void
LoggerEventListener::onPlayerLeave(std::string playerName) {
  std::cout << playerName << " left" << std::endl;
}

void
LoggerEventListener::onHandStart(long handNum, const GameView *game) {
  std::cout << std::endl;
  std::cout << "Starting hand #" << handNum << std::endl;
  game_ = game;  // update just in case
  std::map<size_t, Player> players = game->getPlayers();
  for (auto it = players.begin(); it != players.end(); ++it) {
    std::cout << it->second.getName() << ": " << it->second.getChips() \
              << std::endl;
  }
}

void
LoggerEventListener::onDeal(STREET street) {
  std::cout << "Dealing cards" << std::endl;
  if (street > PREFLOP) {
    std::cout << "board: ";
    std::vector<Card> board = game_->getBoard();
    for (auto it = board.begin(); it != board.end(); ++it) {
      std:: cout << *it;
    }
  std::cout << std::endl;
  }
}

void
LoggerEventListener::onPlayerAction(Action action) {
  std::cout << action << std::endl;
  std::cout << "pot: " << game_->getPot() << std::endl;
}

void
LoggerEventListener::onShowdown(Hand bestHand, std::string playerName) {
  std::cout << playerName << " wins with " << bestHand.str() << std::endl;
}

void
LoggerEventListener::onPotWin(int pot, std::string playerName) {
  std::cout << playerName << " wins " << pot << std::endl;;
}
