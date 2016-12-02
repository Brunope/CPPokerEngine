#ifndef GAME_H_
#define GAME_H_

#include <vector>
#include <cinttypes>
#include <cstddef>
#include <map>

#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Actor.h"
#include "IEventListener.h"
#include "EventManager.h"
#include "Action.h"
#include "GameView.h"

#define STARTING_STACK 1000
#define MAX_NUM_PLAYERS 9

class Game {
public:
  Game(uint32_t big_blind, uint32_t small_blind);

  // Caller retains ownership of 'actor'
  void addPlayer(Actor *actor, std::string name, size_t chips = STARTING_STACK);

  // Caller retains ownership of 'listener'
  void addEventListener(IEventListener *listener);

  void removeEventListener(IEventListener *listener);

  // Play for 'num_hands' hands or until no other players remain. If
  // 'num_hands' is negative, the Game will run until no other players
  // remain.
  void play(int num_hands = -1);

  const GameView &getView() const;
private:
  void removePlayer(const Player &p);
  void updateView();
  void playHand();
  void setupHand();
  void endHand();
  void dealHoleCards();
  void postBlinds();
  void dealNextStreet();
  bool playRound();
  void setupRound();
  void endRound();
  bool handleAction(Action action, Player *source, size_t *num_callers);
  bool isGameOver();
  void updateLegalActions();
  bool isLegalAction(const Action &action);
  bool forceLegalAction(Action *action, Player *source);
  void showdown();
  void showdownNoAllIn();
  void showdownAllIn();
  void showdownWin(const Hand &hand, uint32_t pot, Player *player);
  void playerBet(Player *source, uint32_t chips);
  std::map<size_t, Hand> getPlayerHands();

  static size_t getBestHand(std::map<size_t, Hand> player_hands);

  GameView view_;
  std::map<size_t, Actor *> actors_;
  std::map<size_t, Player> players_;
  std::map<size_t, Player *> live_players_;
  std::map<size_t, Player *> allin_players_;
  std::map<size_t, uint32_t> player_chips_in_pot_per_hand_;
  std::vector<Card> board_;
  std::vector<Action> hand_action_[NUM_STREETS];
  bool legal_actions_[NUM_ACTIONS];
  Deck deck_;
  EventManager eventManager_;
  size_t button_pos_;
  size_t acting_player_seat_;
  uint32_t big_blind_;
  uint32_t small_blind_;
  STREET street_;
  uint32_t hand_num_;
  uint32_t pot_;
  uint32_t current_bet_;
  uint32_t current_raise_by_;
};

#endif  // GAME_H_
