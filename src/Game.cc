#include <cassert>
#include <cstddef>

#include "Player.h"
#include "Actor.h"
#include "Card.h"
#include "Hand.h"
#include "Observer.h"
#include "Actions.h"
#include "Game.h"

Game::Game(uint32_t big_blind, uint32_t small_blind) {
  big_blind_ = big_blind;
  small_blind_ = small_blind;
  button_pos_ = 0;

  updateView();
  notifyObservers();
}

// Setters/Adders

void
Game::addPlayer(Actor *actor, std::string name, size_t chips = STARTING_STACK) {
  assert(getNumPlayers() < MAX_NUM_PLAYERS);
  Player p(name);
  p.chips_ = chips;
  p.seat_ = players_.size();
  actors_.push_back(actor);

  updateView();
  notifyObservers();
}

void
Game::addObserver(Observer *observer) {
  observers_.push_back(observer);
}

const GameView &
Game::getView() const {
  return view_;
}

void
Game::updateView() {
  view_.big_blind_ = big_blind_;
  view.small_blind_ = small_blind_;
  view.button_pos_ = button_pos_;
  view.street_ = street_;
  view.players_ = players_;  // deep copies Players
  // TODO: copy live_players_ but point into view.players_ instead
  view.board_ = board;
  view.hand_action_ = hand_action_;
}

void
Game::play() {
  // TODO: all of this
}
