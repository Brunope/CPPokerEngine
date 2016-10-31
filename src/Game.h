#ifndef GAME_H_
#define GAME_H_

#include <vector>
#include <cinttypes>
#include <cstddef>

#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Actor.h"
#include "Observer.h"
#include "Actions.h"
#include "GameView.h"

#define STARTING_STACK 1000
#define MAX_NUM_PLAYERS 9

class Game {
public:
  Game(uint32_t big_blind, uint32_t small_blind);

  void addPlayer(Actor *actor, std::string name, size_t chips = STARTING_STACK);
  void addObserver(Observer *observer);

  void play();

  const GameView &getView() const;
private:
  void notifyObservers();
  void removePlayer(const Player &p);
  void updateView();

  GameView view_;
  std::vector<Actor *> actors_;         // participants in Game
  std::vector<Player> players_;         // 1-1 index mapping with actors
  std::vector<Player *> live_players_;  // pointers into players_
  std::vector<const Card> board_;
  std::vector<Observer *> observers_;

  size_t button_pos_;
  uint32_t big_blind_;
  uint32_t small_blind_;
  STREET street_;
};

#endif  // GAME_H_
