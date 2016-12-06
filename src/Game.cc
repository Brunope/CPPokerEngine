#include <cassert>
#include <cstddef>
#include <cstdio>

#include "log.h"
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
 *   with the same key, ie actors_[0] is the Actor for players_[0].
 */
Game::Game(uint32_t small_blind, uint32_t big_blind) {
  FILELog::ReportingLevel() = logDEBUG3;
  log_fd_ = fopen("game.log", "w");
  Output2FILE::Stream() = log_fd_;

  small_blind_ = small_blind;
  big_blind_ = big_blind;
  button_pos_ = 0;
  updateView();

  FILE_LOG(logDEBUG) << "Initialized game, blinds " << small_blind_ \
                     << "/" << big_blind_;
}

Game::~Game() {
  fclose(log_fd_);
}

void
Game::addPlayer(Actor *actor, std::string name, size_t chips) {
  assert(players_.size() <= MAX_NUM_PLAYERS);
  Player p(name);
  p.chips_ = chips;
  p.seat_ = players_.size();
  actors_[p.seat_] = actor;
  players_[p.seat_] = p;

  updateView();
  eventManager_.firePlayerJoinEvent(name);

  FILE_LOG(logDEBUG) << "Added " << p.name_ << " in seat " << p.seat_ \
                     << " with " << chips << " chips.";
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

  updateView();
  eventManager_.firePlayerLeaveEvent(player.getName());

  FILE_LOG(logDEBUG) << "Removed player " << player.name_ \
                     << " from seat " << player.seat_;
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
  view_.small_blind_ = small_blind_;
  view_.big_blind_ = big_blind_;
  view_.button_pos_ = button_pos_;
  view_.street_ = street_;
  view_.board_ = board_;
  view_.current_bet_ = current_bet_;
  view_.current_raise_by_ = current_raise_by_;
  view_.acting_player_seat_ = acting_player_seat_;

  view_.players_ = players_;

  // copy all the action over
  view_.hand_action_[PREFLOP] = hand_action_[PREFLOP];
  view_.hand_action_[FLOP] = hand_action_[FLOP];
  view_.hand_action_[TURN] = hand_action_[TURN];
  view_.hand_action_[RIVER] = hand_action_[RIVER];

  FILE_LOG(logDEBUG3) << "Updated view";

  // copy live_players_ but point into view_.players_ instead
  // view_.live_players_ = live_players_;
  // for (auto it = view_.live_players_.begin();
  //      it != view_.live_players_.end(); ++it) {
  //   it->second = &view_.players_[it->second->getSeat()];
  // }

  // // copy allin_players_ but point into view_.players_
  // view_.allin_players_ = allin_players_;
  // for (auto it = view_.allin_players_.begin();
  //      it != view_.allin_players_.end(); ++it) {
  //   it->second = &view_.players_[it->second->getSeat()];
  // }
}

void
Game::play(int num_hands) {
  assert(players_.size() >= 2 && players_.size() <= MAX_NUM_PLAYERS);
  eventManager_.fireGameStartEvent(view_);
  FILE_LOG(logDEBUG) << "playing " << num_hands << " hands";

  hand_num_ = 0;
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
  FILE_LOG(logDEBUG) << "starting hand #" << hand_num_;
  
  postBlinds();
  dealHoleCards();

  // handle the preflop betting round before the loop, since we don't
  // want to deal a street beforehand
  if (playRound()) {
    // everyone but the winner folded, hand ends
    endHand();
    return;
  }
  
  while (street_ < NUM_STREETS) {
    dealNextStreet();
    if (playRound()) {
      endHand();
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
  assert(players_.size() > 1);

  pot_ = 0;
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

  deck_.shuffle();
  street_ = PREFLOP;
  FILE_LOG(logDEBUG2) << "Set up hand, button in seat " << button_pos_;
}

void
Game::endHand() {
  for (auto it = players_.begin(); it != players_.end();) {
    if (it->second.getChips() == 0) {
      FILE_LOG(logDEBUG) << it->second.name_ << " has no more chips"; 
      removePlayer(it->second);
    } else {
      it++;
    }
  }

  hand_num_++;
  button_pos_ = getNextPlayerSeat(button_pos_);

  updateView();
}

void
Game::postBlinds() {
  // post small blind
  Action post_action;
  bool illegal_action;
  // Special case with only two players - the button is the small
  // blind, and the other player is the big blind. The button
  // starts the action
  if (live_players_.size() == 2) {
    acting_player_seat_ = button_pos_;
  } else {
    acting_player_seat_ = getNextLivePlayerSeat(button_pos_);
  }
  
  Player *player = live_players_[acting_player_seat_];
  if (player->getChips() >= small_blind_) {
    post_action = Action(POST, small_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
    FILE_LOG(logDEBUG1) << player->name_ << " doesn't have enough " \
                        << "chips for small blind";
  }

  FILE_LOG(logDEBUG1) << "Posting small blind for " << player->name_;

  illegal_action = handleAction(post_action, player);
  assert(!illegal_action);

  // post big blind
  player = live_players_[acting_player_seat_];
  if (player->getChips() >= big_blind_) {
    post_action = Action(POST, big_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
  }

  FILE_LOG(logDEBUG1) << "Posting big blind for " << player->name_;
  
  illegal_action = handleAction(post_action, player);
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
  size_t num_callers_ = 1;
  while (num_callers_ < live_players_.size()) {
    
    FILE_LOG(logDEBUG1) << num_callers_ << " callers so far, " \
                        << live_players_.size() << " players";
    
    current_player = live_players_[acting_player_seat_];
    if (current_player->isAllIn()) {
      num_callers_++;
      acting_player_seat_ = getNextLivePlayerSeat(acting_player_seat_);
      FILE_LOG(logDEBUG2) << "Skipping all in player " << current_player->name_;
      continue;
    }
    current_actor = actors_[acting_player_seat_];
    
    FILE_LOG(logDEBUG1) << "Asking " << current_player->name_ \
                        << " for action";
    
    current_action = current_actor->act(view_);
    handleAction(current_action, current_player);
  }

  // uhhhahaha
  endRound();

  if (live_players_.size() == 1) {
    // give chips to the winner, the only remaining entry in live_players_
    potWin(pot_, live_players_.begin()->second);
    return true;
  }
  return false;
}

// set acting_player_seat_ to one seat left of the button, zero out
// chips in play for all players, not just live ones.
void
Game::setupRound() {
  // preflop, the blinds have already been posted before the round starts,
  // so the current actor is already initialized to the player after the
  // big blind.
  if (street_ > PREFLOP) {
    acting_player_seat_ = getNextLivePlayerSeat(button_pos_);
  }
  
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    it->second->chips_in_play_ = 0;
  }
}

// Don't actually need to do anything here I don't think
void
Game::endRound() {

}  

// return true if action was originally illegal and changed to
// a legal action (fold or check)
bool
Game::handleAction(Action action, Player *source) {
  // ensure action is legal, and has proper source player
  updateLegalActions();
  action = Action(action.getType(), action.getAmount(), source);
  bool action_changed = forceLegalAction(&action, source);

  FILE_LOG(logDEBUG1) << "Handling " << action.getType() << ", " \
                      << action.getAmount() << " from " << source->name_;

  switch (action.getType()) {
  case FOLD:
    live_players_.erase(acting_player_seat_);
    FILE_LOG(logDEBUG1) << source->name_ << " folded, no longer live";
    break;
  case CHECK:
    (num_callers_)++;
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
    num_callers_ = 1;
    // since the action's amount is the amount to raise to, if the
    // player already has chips in the pot (maybe because of a previous
    // raise), the additional chips to add to the pot is the difference
    // between the amount and the chips already committed.
    playerBet(source, action.getAmount() - source->getChipsInPlay());
    break;
  case CALL:
    (num_callers_)++;
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
    num_callers_ = 1;
    playerBet(source, action.getAmount());
    break;
  default:
    assert(false);
    break;
  }

  hand_action_[street_].push_back(action);
  acting_player_seat_ = getNextLivePlayerSeat(acting_player_seat_);
  
  updateView();
  eventManager_.firePlayerActionEvent(action);

  FILE_LOG(logDEBUG2) << "Next actor in seat " << acting_player_seat_;

  return action_changed;
}

// return the next filled seat following 'seat'
size_t
Game::getNextPlayerSeat(size_t seat) {
  size_t next_seat = (seat + 1) % MAX_NUM_PLAYERS;
  while (!players_.count(next_seat)) {
    next_seat = (next_seat + 1) % MAX_NUM_PLAYERS;
  }
  return next_seat;
}

size_t
Game::getNextLivePlayerSeat(size_t seat) {
  size_t next_seat = (seat + 1) % MAX_NUM_PLAYERS;
  while (!live_players_.count(next_seat)) {
    next_seat = (next_seat + 1) % MAX_NUM_PLAYERS;
  }
  return next_seat;
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

  updateView();
  
  FILE_LOG(logDEBUG1) << "Added " << chips << " chips to pot from "  \
                      << player->name_;
}
  
// return true if action was modified
bool
Game::forceLegalAction(Action *action, Player *source) {
  assert(source);
  FILE_LOG(logDEBUG2) << "Force legal action";
  if (!isLegalAction(*action)) {
    FILE_LOG(logDEBUG1) << "Action " << action->getType() << " illegal";
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

  FILE_LOG(logDEBUG2) << "Updating legal actions for " << player.name_;

  bool can_raise = current_bet_ - player.getChipsInPlay() < player.getChips();

  // A post must be the first or second action of the hand
  // for now ignore the case where a blind puts player all in
  if (street_ == PREFLOP && hand_action_[PREFLOP].size() < 2) {
    legal_actions_[RAISE] = false;
    legal_actions_[CALL] = false;
    legal_actions_[FOLD] = false;
    legal_actions_[CHECK] = false;
    legal_actions_[POST] = true;

    FILE_LOG(logDEBUG2) << "Allow POST";
  } else {
    Action &last_action = hand_action_[street_].back();
    assert(last_action.getType() < NUM_ACTIONS);

    if (last_action.getType() == CHECK) {
      legal_actions_[RAISE] = can_raise;
      legal_actions_[CALL] = false;
      legal_actions_[FOLD] = false;
      legal_actions_[CHECK] = true;
      legal_actions_[POST] = false;

      FILE_LOG(logDEBUG2) << "Allow CHECK";
    } else {
      // action.getType() == RAISE, CALL, FOLD, or POST
      legal_actions_[RAISE] = can_raise;
      legal_actions_[CALL] = true;
      legal_actions_[FOLD] = true;
      legal_actions_[CHECK] = false;
      legal_actions_[POST] = false;

      FILE_LOG(logDEBUG2) << "Allow FOLD";
    }

    // There's a special case where everyone simply calls the big blind,
    // and the player in the big blind then has the option to check or
    // raise but not call. Or maybe a player joins and opts to
    // post their blind immediately instead of waiting.
    if (current_bet_ == player.getChipsInPlay()) {
      legal_actions_[RAISE] = can_raise;
      legal_actions_[CALL] = false;
      legal_actions_[FOLD] = false;
      legal_actions_[CHECK] = true;
      legal_actions_[POST] = false;

      FILE_LOG(logDEBUG2) << "Allow CHECK";
    }
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
  potWin(pot, player);
}

void
Game::potWin(uint32_t pot, Player *player) {
  player->chips_ += pot;
  updateView();
  eventManager_.firePotWinEvent(pot, player->name_);
  FILE_LOG(logDEBUG) << player->name_ << " won " << pot;
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
