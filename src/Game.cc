#include <cassert>
#include <cstddef>

#include "Player.h"
#include "Actor.h"
#include "Card.h"
#include "Hand.h"
#include "Observer.h"
#include "Action.h"
#include "Game.h"


/**
 * Whole bunch of invariants
 *
 * players_
 * - Every Player in 'players_' is a current participant in the Game. There
 *   is no such thing as an inactive Player; it is impossible for players to
 *   sit out hands. 
 * - Each Player in 'players_' has a nonzero chip count. As soon as a Player
 *   loses all their chips, they are removed from 'players_'.
 * - Each Player in 'players_' is located at the index equal to their seat
 *   number, Player::getSeat().
 *
 * actors_
 * - Each Actor * in 'actors_' is associated with the Player in 'players_'
 *   at the same index, ie actors_[0] is the Actor for players_[0].
 * - Since there is a 1-1 mapping, the same invariants for players_ holds
 *   for actors_.
 */
Game::Game(uint32_t big_blind, uint32_t small_blind) {
  big_blind_ = big_blind;
  small_blind_ = small_blind;
  button_pos_ = 0;

  updateView();
}

void
Game::addPlayer(Actor *actor, std::string name, size_t chips) {
  assert(players_.size() <= MAX_NUM_PLAYERS);
  Player p(name);
  p.chips_ = chips;
  p.seat_ = players_.size();
  actors_[p.seat_] = actor;

  updateView();
  eventManager_.firePlayerJoinEvent(name);
}

// Remove the Player at seat from players_ and live_players_, and remove
// its Actor from actors_.
void
Game::removePlayer(const Player &player) {
  actors_.erase(player.getSeat());
  players_.erase(player.getSeat());
  if (live_players_.count(player.getSeat())) {
    live_players_.erase(player.getSeat());
  }
  if (allin_players_.count(player.getSeat())) {
    allin_players_.erase(player.getSeat());
  }
}

void
Game::addEventListener(IEventListener *listener) {
  eventManager_.addEventListener(listener);
}

void
Game::removeEventListener(IEventListener *listener) {
  eventManager_.removeEventListener(listener);
}

const GameView &
Game::getView() const {
  return view_;
}

void
Game::updateView() {
  // TODO: make a million times faster
  view_.big_blind_ = big_blind_;
  view_.small_blind_ = small_blind_;
  view_.button_pos_ = button_pos_;
  view_.street_ = street_;
  view_.board_ = board_;
  view_.players_ = players_;

  // copy all the action over
  view_.hand_action_[PREFLOP] = hand_action_[PREFLOP];
  view_.hand_action_[FLOP] = hand_action_[FLOP];
  view_.hand_action_[TURN] = hand_action_[TURN];
  view_.hand_action_[RIVER] = hand_action_[RIVER];

  // copy live_players_ but point into view_.players_ instead
  view_.live_players_ = live_players_;
  for (auto it = view_.live_players_.begin();
       it != view_.live_players_.end(); ++it) {
    it->second = &view_.players_[it->second->getSeat()];
  }

  // copy allin_players_ but point into view_.players_
  view_.allin_players_ = allin_players_;
  for (auto it = view_.allin_players_.begin();
       it != view_.allin_players_.end(); ++it) {
    it->second = &view_.players_[it->second->getSeat()];
  }
}

void
Game::play(int num_hands) {
  assert(players_.size() >= 2 && players_.size() <= MAX_NUM_PLAYERS);
  eventManager_.fireGameStartEvent(view_);
  while (!isGameOver() && hand_num_ != num_hands) {
    playHand();
  }
  
  eventManager_.fireGameOverEvent(view_);
}

void
Game::playHand() {
  // reset a bunch of variables, and add all player pointers to the live list
  setupHand();

  eventManager_.fireHandStartEvent(hand_num_, view_);

  postBlinds();
  dealHoleCards();

  // handle the preflop betting round before the loop, since we don't
  // want to deal a street beforehand
  if (!playRound()) {
    // everyone but the winner folded, hand ends
    return;
  }
  
  while (street_ < NUM_STREETS) {
    dealNextStreet();
    if (!playRound()) {
      return;
    }
  }

  // if the loop terminated, action made it all the way through the
  // river to showdown
  showdown();

  // remove any players that lost all chips
  endHand();
}

void
Game::setupHand() {
  board_.clear();
  allin_players_.clear();
  live_players_.clear();
  player_chips_in_pot_per_hand_.clear();

  // add all players to live_players_, zero out chips_in_play
  Player *player;
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    player = &(it->second);
    assert(player->getChips() > 0);
    player->chips_in_play_ = 0;
    live_players_[it->first] = player;
    player_chips_in_pot_per_hand_[it->first] = 0;
  }

  // clear hand_action_ from previous hand
  hand_action_[PREFLOP].clear();
  hand_action_[FLOP].clear();
  hand_action_[TURN].clear();
  hand_action_[RIVER].clear();

  // rotate button around the table
  button_pos_ = (button_pos_ + 1) % players_.size();

  // action starts to the left of the button, I guess left is positive
  acting_player_seat_ = (button_pos_ + 1) % live_players_.size();

  deck_.shuffle();
  street_ = PREFLOP;
}

void
Game::endHand() {
  for (auto it = players_.begin(); it != players_.end();) {
    if (it->second.getChips() == 0) {
      removePlayer(it->second);
    } else {
      it++;
    }
  }

  hand_num_++;
}

void
Game::postBlinds() {
  // post small blind
  Action post_action;
  bool illegal_action;
  Player *player = live_players_[acting_player_seat_];
  if (player->getChips() >= small_blind_) {
    post_action = Action(POST, small_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
  }

  size_t throwaway = 0;
  illegal_action = handleAction(post_action, player, &throwaway);
  assert(!illegal_action);

  // post big blind
  player = live_players_[acting_player_seat_];
  if (player->getChips() >= big_blind_) {
    post_action = Action(POST, big_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
  }
  illegal_action = handleAction(post_action, player, &throwaway);
  assert(!illegal_action);
}

void
Game::dealHoleCards() {
  assert(street_ == PREFLOP);
  for (size_t seat = 0; seat < players_.size(); seat++) {
    players_[seat].hc_ = deck_.dealHoleCards();
  }
  eventManager_.fireDealEvent(PREFLOP);
}

// increment street_, then deal that street
void
Game::dealNextStreet() {
  assert(street_ <= RIVER);

  street_ = static_cast<STREET>(static_cast<int>(street_) + 1);  // sorry
  // deal 3 cards for the flop, 1 card everywhere else
  if (street_ == PREFLOP) {
    board_.push_back(deck_.dealNextCard());
    board_.push_back(deck_.dealNextCard());
    board_.push_back(deck_.dealNextCard());
  } else {
    board_.push_back(deck_.dealNextCard());
  }

  eventManager_.fireDealEvent(street_);
}

// return true if the hand is over because everyone folded but the winner
bool
Game::playRound() {
  // start actor to the left of the button, clear any outstanding bets,
  // zero out chips in play of all players, including those who have folded
  setupRound();
  
  Player *current_player;
  Actor *current_actor;
  Action current_action;
  size_t num_callers = 0;
  while (num_callers < live_players_.size()) {
    current_player = live_players_[acting_player_seat_];
    if (current_player->isAllIn()) {
      num_callers++;
      continue;
    }
    current_actor = actors_[acting_player_seat_];
    current_action = current_actor->act(view_);
    handleAction(current_action, current_player, &num_callers);
  }

  // uhhhahaha
  endRound();

  if (live_players_.size() == 1) {
    return true;
  }
  return false;
}

// set acting_player_seat_ to one seat left of the button, zero out
// chips in play for all players, not just live ones.
void
Game::setupRound() {
  acting_player_seat_ = (button_pos_ + 1) % live_players_.size();
  for (size_t seat = 0; seat < players_.size(); seat++) {
    players_[seat].chips_in_play_ = 0;
  }
}

// Don't actually need to do anything here I don't think
void
Game::endRound() {

}  

// return true if action was originally illegal and changed to
// a legal action (fold or check)
bool
Game::handleAction(Action action, Player *source, size_t *num_callers) {
  // ensure action is legal, and has proper source player
  updateLegalActions();
  action = Action(action.getType(), action.getAmount(), source);
  bool action_changed = forceLegalAction(&action, source);

  switch (action.getType()) {
  case FOLD:
    live_players_.erase(acting_player_seat_);
    // elements shift over on erase, so if we didn't decrement
    // here, the increment after the switch would skip an actor
    acting_player_seat_ = (acting_player_seat_ - 1) % live_players_.size();
    break;
  case CHECK:
    (*num_callers)++;
    break;
  case RAISE:
    // can be greater if source player raises twice, second time all in
    if (action.getAmount() > source->getChips()) {
      assert(action.getAmount() ==
             source->getChips() + source->getChipsInPlay());
      allin_players_[acting_player_seat_] = source;
    } else {
      // must raise by at least twice the amount the previous raise
      // increased the pot
      assert(action.getAmount() >= current_bet_ + current_raise_by_);
    }
    current_raise_by_ = action.getAmount() - current_bet_;
    current_bet_ = action.getAmount();
    // new highest bet, everyone has to call again
    *num_callers = 1;
    // since the action's amount is the amount to raise to, if the
    // player already has chips in the pot (maybe because of a previous
    // raise), the additional chips to add to the pot is the difference
    // between the amount and the chips already committed.
    playerBet(source, action.getAmount() - source->getChipsInPlay());
    break;
  case CALL:
    (*num_callers)++;
    // confusing, but for calls, the amount is already the difference
    // in chips to call
    playerBet(source, action.getAmount());
    break;
  case POST:
    // could be less than small blind too, if player is all in
    assert(action.getAmount() <= big_blind_);
    // poker rule: players must call the full amount of the big blind,
    // even if the player in the big blind did not have enough chips
    // to post the full amount.
    current_bet_ = big_blind_;
    current_raise_by_ = big_blind_;
    *num_callers = 1;
    playerBet(source, action.getAmount());
    break;
  default:
    assert(false);
    break;
  }

  hand_action_[street_].push_back(action);
  acting_player_seat_ = (acting_player_seat_ + 1) % live_players_.size();
  eventManager_.firePlayerActionEvent(action);

  return action_changed;
}

// add player chips to the pot
void
Game::playerBet(Player *player, uint32_t chips) {
  assert(player->chips_ >= chips);  // reaaaally don't wanna underflow
  assert(live_players_[acting_player_seat_] == player);
  
  // player all in
  if (player->chips_ == chips) {
    allin_players_[player->getSeat()] = player;
  }

  player->chips_ -= chips;
  player->chips_in_play_ += chips;
  player_chips_in_pot_per_hand_[player->getSeat()] += chips;
  pot_ += chips;
}
  
// return true if action was modified
bool
Game::forceLegalAction(Action *action, Player *source) {
  assert(source);
  if (!isLegalAction(*action)) {
    if (legal_actions_[CHECK]) {
      *action = Action(CHECK, 0, source);
    } else {
      assert(legal_actions_[FOLD]);
      *action = Action(FOLD, 0, source);
    }
    return true;
  }
  return false;
}

bool
Game::isLegalAction(const Action &action) {
  if (action.getType() == RAISE) {
    return (legal_actions_[RAISE] &&
            (action.getAmount() - action.getSource()->getChipsInPlay()) >
            action.getSource()->getChips());
  } else {
    return legal_actions_[action.getType()];
  }
}

// must have acting_player_seat_ pointing to the player about to act
void
Game::updateLegalActions() {
  const Player &player = players_[acting_player_seat_];
  bool can_raise = current_bet_ - player.getChipsInPlay() < player.getChips();

  // A post must be the first or second action of the hand
  // for now ignore the case where a blind puts player all in
  if (street_ == PREFLOP && hand_action_[PREFLOP].size() < 2) {
    legal_actions_[RAISE] = false;
    legal_actions_[CALL] = false;
    legal_actions_[FOLD] = false;
    legal_actions_[CHECK] = false;
    legal_actions_[POST] = true;
  }

  Action &last_action = hand_action_[street_].back();
  assert(last_action.getType() < NUM_ACTIONS);

  if (last_action.getType() == CHECK) {
    legal_actions_[RAISE] = can_raise;
    legal_actions_[CALL] = false;
    legal_actions_[FOLD] = false;
    legal_actions_[CHECK] = true;
    legal_actions_[POST] = false;
  } else {
    // action.getType() == RAISE, CALL, FOLD, or POST
    legal_actions_[RAISE] = can_raise;
    legal_actions_[CALL] = true;
    legal_actions_[FOLD] = true;
    legal_actions_[CHECK] = false;
    legal_actions_[POST] = false;
  }

  // There's a special case where everyone simply calls the big blind,
  // and the player in the big blind then has the option to check or
  // raise but not call. The player in the big blind is two seats after
  // the dealer.
  if (current_bet_ == big_blind_ &&
      acting_player_seat_ == (button_pos_ + 2) % players_.size()) {
    legal_actions_[RAISE] = can_raise;  // think this is always true
    legal_actions_[CALL] = false;
    legal_actions_[FOLD] = false;
    legal_actions_[CHECK] = true;
    legal_actions_[POST] = false;
  }
}

void
Game::showdown() {
  // find player with the best hand
  if (allin_players_.empty()) {
    showdownNoAllIn();
  } else {
    showdownAllIn();
  }
}

// live player with best hand wins entire pot_
void
Game::showdownNoAllIn() {
  std::map<size_t, Hand> live_player_hands = getPlayerHands();
  size_t winning_player_seat = getBestHand(live_player_hands);
  showdownWin(live_player_hands[winning_player_seat],
              pot_, &players_[winning_player_seat]);
}

// Enforce that a player can only win up to what they have contributed to the
// pot from each other player. Pay out to the best hands until the entire
// pot is used up.
void
Game::showdownAllIn() {
  // map seat numbers to winnings for this hand
  std::map<size_t, uint32_t> player_winnings;
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    player_winnings[it->first] = 0;
  }

  std::map<size_t, Hand> live_player_hands = getPlayerHands();
  std::map<size_t, Hand> remaining_player_hands = live_player_hands;
  std::map<size_t, uint32_t> player_contrib = player_chips_in_pot_per_hand_;
  uint32_t remaining_pot = pot_;
  uint32_t chips_won, chips_can_win;
  size_t winning_player_seat;
  while (remaining_pot) {
    winning_player_seat = getBestHand(remaining_player_hands);

    // subtract the amount of chips the player can win from each other
    // player's contribution to the pot
    chips_can_win = player_contrib[winning_player_seat];
    chips_won = chips_can_win;
    for (auto it = player_contrib.begin(); it != player_contrib.end(); ++it) {
      if (it->first != winning_player_seat) {
        if (it->second >= chips_can_win) {
          chips_won += chips_can_win;
          it->second -= chips_can_win;
        } else {
          chips_won += it->second;
          it->second = 0;
        }
      }
    }
    
    player_winnings[winning_player_seat] += chips_won;
    assert(chips_won <= remaining_pot);
    remaining_pot -= chips_won;
    remaining_player_hands.erase(winning_player_seat);
  }

  for (auto it = player_winnings.begin(); it != player_winnings.end(); ++it) {
    if (it->second > 0) {  // player actually won some chips
      showdownWin(live_player_hands[it->first], it->second,
                  &players_[it->first]);
    }
  }
}

void 
Game::showdownWin(const Hand &hand, uint32_t pot, Player *player) {
  eventManager_.fireShowdownEvent(hand, player->getName());
  player->chips_ += pot;
  eventManager_.firePotWinEvent(pot, player->getName());
}

std::map<size_t, Hand>
Game::getPlayerHands() {
  std::map<size_t, Hand> player_hands;
  std::vector<Card> player_cards = board_;
  player_cards.resize(board_.size() + 2);  // make space for 2 hole cards
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    // add player's first hole card
    player_cards[board_.size()] = it->second->hc_.first;
    // second hole card
    player_cards[board_.size() + 1] = it->second->hc_.second;
    player_hands[it->first] = Hand(player_cards);
  }
  return player_hands;
}

size_t
Game::getBestHand(std::map<size_t, Hand> player_hands) {
  int best_hand_strength = -1;
  size_t best_hand_seat = MAX_NUM_PLAYERS;  // inval
  for (auto it = player_hands.begin(); it != player_hands.end(); ++it) {
    if (it->second.eval() > best_hand_strength) {
      best_hand_strength = it->second.eval();
      best_hand_seat = it->first;
    }
  }

  assert(best_hand_seat < MAX_NUM_PLAYERS);
  return best_hand_seat;
}

// The game is over if only one player remains with nonzero chip count
// As soon as a player loses all chips, they are removed from the Game
// so the number of players with nonzero chip counts is just the number
// of players
bool
Game::isGameOver() {
  return players_.size() == 1;
}
