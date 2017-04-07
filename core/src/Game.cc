#include <cassert>
#include <cstddef>
#include <cstdio>
#include <algorithm>

#include "log.h"
#include "Player.h"
#include "Agent.h"
#include "Card.h"
#include "Hand.h"
#include "Action.h"
#include "HandHistory.h"
#include "Random.h"
#include "Game.h"


/**
 * Whole bunch of invariants
 *
 * players_
 * - Each Player in players_ is a current participant in the Game. There
 *   is no such thing as an inactive Player; it is impossible for players to
 *   sit out hands. 
 * - Each Player in players_ has a nonzero chip count. As soon as a Player
 *   loses all their chips, they are removed from players_.
 * - Each Player in players_ is located at the index equal to their seat
 *   number, Player::getSeat().
 * - live_players_ holds 1 pointer to each Player in players_ with 
 *   player::isLive == true.
 * - allin_players holds 1 pointer to each Players in players_ with
 *   player::isAllIn() == true.
 *
 * agents_
 * - Each Agent * in agents_ is associated with the Player in players_
 *   with the same key, ie agents_[0] is the Agent for players_[0].
 *
 * TODO: timeout on act
 * TODO: stop()
 * TODO: config file specify log file name
 */
Game::Game(uint32_t small_blind, uint32_t big_blind) {
  Random::seed();
  
  FILELog::ReportingLevel() = logDEBUG2;
  log_fd_ = fopen("game.log", "w");
  if (log_fd_ == nullptr) {
    std::cerr << "Couldn't open Game log file" << std::endl;
  }
  Output2FILE::Stream() = log_fd_;

  view_ = std::shared_ptr<GameView>(new GameView());
  const_view_ = std::const_pointer_cast<const GameView>(view_);
  
  small_blind_ = small_blind;
  big_blind_ = big_blind;
  button_seat_ = 0;
  hand_num_ = 0;
  updateView();

  FILE_LOG(logDEBUG) << "Initialized game, blinds " << small_blind_ \
                     << "/" << big_blind_ << ", seed " << Random::getSeed();
}

Game::~Game() {
  FILE_LOG(logDEBUG2) << "Destructing Game";
  if (log_fd_ != nullptr) {
    FILE_LOG(logDEBUG3) << "closing log file";
    fclose(log_fd_);
  }
}

void
Game::addPlayer(std::shared_ptr<Agent> agent, std::string name, size_t chips) {
  assert(players_.size() <= MAX_NUM_PLAYERS);
  Player player(name);
  player.chips_ = chips;
  player.seat_ = players_.size();
  agents_[player.seat_] = agent;
  players_[player.seat_] = player;

  updateView();
  event_manager_.firePlayerJoinEvent(player);

  FILE_LOG(logDEBUG) << "Added " << player.name_ << " in seat " \
                     << player.seat_ << " with " << chips << " chips.";
}

// Remove the Player at player.getSeat() from players_ and live_players_, and
// remove its Agent from agents_. You shouldn't call this method directly,
// rather call removePlayer(seat) which calls this.
void
Game::removePlayer(Player player) {
  agents_.erase(player.getSeat());
  players_.erase(player.getSeat());
  if (live_players_.count(player.getSeat())) {
    live_players_.erase(player.getSeat());
  }
  if (allin_players_.count(player.getSeat())) {
    allin_players_.erase(player.getSeat());
  }

  updateView();
  event_manager_.firePlayerLeaveEvent(player);

  FILE_LOG(logDEBUG) << "Removed " << player.name_ << " from seat " \
                     << player.seat_;
}

void
Game::removePlayer(size_t seat) {
  if (players_.count(seat)) {
    removePlayer(players_.at(seat)); 
  }
}

void
Game::addEventListener(std::shared_ptr<IEventListener> listener) {
  event_manager_.addEventListener(listener);
}

void
Game::removeEventListener(std::shared_ptr<IEventListener> listener) {
  event_manager_.removeEventListener(listener);
}

std::shared_ptr<const GameView>
Game::getView() const {
  return const_view_;
}

void
Game::updateView() {
  // TODO: incremental view updates
  view_->small_blind_ = small_blind_;
  view_->big_blind_ = big_blind_;
  view_->button_pos_ = button_seat_;  // todo
  view_->street_ = street_;
  view_->board_ = board_;
  view_->current_bet_ = current_bet_;
  view_->current_raise_by_ = current_raise_by_;
  view_->pot_ = pot_;
  view_->acting_player_seat_ = acting_player_seat_;
  view_->hand_num_ = hand_num_;

  view_->players_ = players_;

  // copy hand history
  view_->history_ = history_;

  // copy all legal actions
  for (uint8_t type = 0; type < NUM_ACTIONS; type++) {
    view_->legal_actions_[type] = legal_actions_[type];
  }

  // give all agents pointers to their copy of player stored in the view
  // shouldn't have to do this so often...
  for (auto it = agents_.begin(); it != agents_.end(); ++it) {
    it->second->setPlayer(&(view_->players_.at(it->first)));
  }

  FILE_LOG(logDEBUG3) << "pot_: " << view_->pot_;
  FILE_LOG(logDEBUG3) << "current_bet_: " << view_->current_bet_;
  FILE_LOG(logDEBUG3) << "current_raise_by_: " << view_->current_raise_by_;
  FILE_LOG(logDEBUG3) << "Updated view";
}

void
Game::play(int num_hands) {
  assert(players_.size() >= 2 && players_.size() <= MAX_NUM_PLAYERS);
  event_manager_.fireGameStartEvent(const_view_);
  FILE_LOG(logDEBUG) << "playing " << num_hands << " hands";

  num_hands += hand_num_;
  while (!isGameOver() && hand_num_ != num_hands) {
    playHand();
  }

  if (isGameOver()) {
    event_manager_.fireGameEndEvent();
  }
}

void
Game::playHand() {
  // reset a bunch of variables, and add all player pointers to the live list
  setupHand();

  event_manager_.fireHandStartEvent(hand_num_, const_view_);
  
  FILE_LOG(logDEBUG) << "starting hand #" << hand_num_;
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    FILE_LOG(logDEBUG1) << it->second.name_ << ": " << it->second.chips_;
  }
      
  postBlinds();
  dealHoleCards();

  // handle the preflop betting round before the loop, since we don't
  // want to deal a street beforehand
  if (playRound()) {
    // everyone but the winner folded, hand ends
    endHand();
    return;
  }

  // dealNextStreet increments street_ then deals the new street,
  // to keep street_ consistent with the current state of the board.
  // So, we don't want to deal another street after we reach the river
  while (street_ < RIVER) {
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
    player->live_ = true;
    live_players_[it->first] = player;
    player_chips_in_pot_per_hand_[it->first] = 0;
  }

  // clear hand_action_ from previous hand
  history_.hand_action_[PREFLOP].clear();
  history_.hand_action_[FLOP].clear();
  history_.hand_action_[TURN].clear();
  history_.hand_action_[RIVER].clear();
  history_.known_hands_.clear();
  history_.player_winnings_.clear();

  deck_.shuffle7();  // extra extra random namsayin
  street_ = PREFLOP;

  // Set small blind and big blind seats. Special case with only two players,
  // the button is actually the small blind and the other player is the big
  // blind. Otherwise, the small blind is directly to the left of the dealer,
  // and the big blind is to the left of the small blind.
  if (live_players_.size() == 2) {
    sb_seat_ = button_seat_;
    bb_seat_ = getNextLivePlayerSeat(sb_seat_);
  } else {
    sb_seat_ = getNextLivePlayerSeat(button_seat_);
    bb_seat_ = getNextLivePlayerSeat(sb_seat_);
  }

  updateView();
  FILE_LOG(logDEBUG2) << "Set up hand, button in seat " << button_seat_;
}

// Give each agent the hand history of the hand that just ended,
// and remove any player with no more chips
void
Game::endHand() {
  for (auto it = players_.begin(); it != players_.end();) {
    if (it->second.getChips() == 0) {
      FILE_LOG(logDEBUG) << it->second.name_ << " has no more chips";
      // remove from players_ here, cause we need the next iterator
      Player to_remove = it->second;
      it = players_.erase(it);
      removePlayer(to_remove);
    } else {
      agents_[it->first]->receiveHandHistory(view_->history_);
      ++it;
    }
  }

  hand_num_++;
  button_seat_ = getNextPlayerSeat(button_seat_);

  updateView();
}

void
Game::postBlinds() {
  // post small blind
  Action post_action;
  bool illegal_action;

  acting_player_seat_ = sb_seat_;
  Player *player = live_players_[acting_player_seat_];
  if (player->getChips() >= small_blind_) {
    post_action = Action(POST, small_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
    FILE_LOG(logDEBUG1) << player->name_ << " doesn't have enough " \
                        << "chips for small blind";
  }

  illegal_action = handleAction(post_action, player);
  assert(!illegal_action);

  // post big blind
  player = live_players_[acting_player_seat_];
  if (player->getChips() >= big_blind_) {
    post_action = Action(POST, big_blind_, player);
  } else {
    post_action = Action(POST, player->getChips(), player);
  }

  illegal_action = handleAction(post_action, player);
  assert(!illegal_action);
}

void
Game::dealHoleCards() {
  assert(street_ == PREFLOP);
  hole_cards_.clear();
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    std::pair<Card, Card> hc = deck_.dealHoleCards();
    hole_cards_[it->first] = hc;
    agents_[it->first]->receiveHoleCards(hc);
    FILE_LOG(logDEBUG1) << "Dealt " << it->second.name_ << " " \
                        << hc.first << hc.second;
  }
  event_manager_.fireDealEvent(PREFLOP);
}

// increment street_, then deal that street
// requires street < RIVER
void
Game::dealNextStreet() {
  assert(street_ < RIVER);

  street_ = static_cast<STREET>(static_cast<int>(street_) + 1);  // sorry
  // deal 3 cards for the flop, 1 card everywhere else
  if (street_ == FLOP) {
    board_.push_back(deck_.dealNextCard());
    board_.push_back(deck_.dealNextCard());
    board_.push_back(deck_.dealNextCard());

    FILE_LOG(logDEBUG1) << "flop: " << board_[0] << board_[1] << board_[2];
  } else {
    board_.push_back(deck_.dealNextCard());

    if (street_ == TURN) {
      FILE_LOG(logDEBUG1) << "turn: " << board_[3];
    } else {
      FILE_LOG(logDEBUG1) << "river: " << board_[4];
    }
  }

  updateView();
  event_manager_.fireDealEvent(street_);
}

// return true if the hand is over because everyone folded but the winner
bool
Game::playRound() {
  // start agent to the left of the button, clear any outstanding bets,
  // zero out chips in play of all players, including those who have folded
  setupRound();
  
  Player *current_player;
  std::shared_ptr<Agent> current_agent;
  Action current_action;

  // preflop, blinds set num_callers_
  if (street_ > PREFLOP) {
    num_callers_ = 0;
  }

  // Every live player that is not all in must act at least once.
  // The betting continues until all players have put equal chips
  // into the pot, ie chips_in_play_ is equal for all players. This
  // is the case when num_callers == live_players_.size(). The
  // second loop condition skips the betting round when there is only
  // one non-allin player.
  size_t num_allin_at_start = allin_players_.size();
  while (num_callers_ < live_players_.size() &&
         live_players_.size() - num_allin_at_start > 1) {
    
    FILE_LOG(logDEBUG2) << num_callers_ << " callers so far, " \
                        << live_players_.size() << " players";
    
    current_player = live_players_[acting_player_seat_];
    if (current_player->isAllIn()) {
      num_callers_++;
      acting_player_seat_ = getNextLivePlayerSeat(acting_player_seat_);
      FILE_LOG(logDEBUG2) << "Skipping all in player " << current_player->name_;
      continue;
    }

    current_agent = agents_[acting_player_seat_];
    updateView();
    event_manager_.fireViewChangedEvent(const_view_);
    
    FILE_LOG(logDEBUG2) << "Asking " << current_player->name_ \
                        << " for action";
    current_action = current_agent->act(*view_);
    handleAction(current_action, current_player);
  }

  // Reset current_bet_ and current_raise_by_
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
  // so the current agent is already initialized to the player after the
  // big blind. Don't just set it to 3 left of the button because
  // that misses the special case of 2 players where the button acts first.
  if (street_ > PREFLOP) {
    acting_player_seat_ = getNextLivePlayerSeat(button_seat_);
  }

  // Clear chips in play
  // Important! Don't clobber chips in play for the small blind and
  // big blind players preflop, for the same reason as before
  for (auto it = players_.begin(); it != players_.end(); ++it) {
    if (street_ == PREFLOP &&
        (it->first == sb_seat_ || it->first == bb_seat_)) {
      continue;
    }
    it->second.chips_in_play_ = 0;
  }
}

// Reset current_bet_ and current_raise_by_, set acting_player_seat to invalid
void
Game::endRound() {
  current_bet_ = 0;
  current_raise_by_ = big_blind_;
  acting_player_seat_ = MAX_NUM_PLAYERS;
  FILE_LOG(logDEBUG2) << "End round " << street_;
}  

// return true if action was originally illegal and changed to
// a legal action (fold or check)
bool
Game::handleAction(Action action, Player *source) {
  // ensure action is legal, and has proper source player
  updateLegalActions();
  action = Action(action.getType(), action.getAmount(), source);
  bool action_changed = forceLegalAction(&action, source);

  FILE_LOG(logDEBUG1) << action;

  switch (action.getType()) {
  case FOLD:
    source->live_ = false;
    live_players_.erase(acting_player_seat_);
    FILE_LOG(logDEBUG2) << source->name_ << " folded, no longer live";
    break;
  case CHECK:
    num_callers_++;
    break;
  case RAISE:
    // can be greater if source player raises twice, second time all in
    if (action.getAmount() >= source->getChips()) {
      assert(action.getAmount() <=
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
    FILE_LOG(logDEBUG3) << source->name_ << " has " << source->chips_in_play_ \
                        << " chips in play";
    // since the action's amount is the amount to raise to, if the
    // player already has chips in the pot (maybe because of a previous
    // raise), the additional chips to add to the pot is the difference
    // between the amount and the chips already committed.
    playerBet(source, action.getAmount() - source->getChipsInPlay());
    break;
  case CALL:
    num_callers_++;
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
    num_callers_ = 0;
    playerBet(source, action.getAmount());
    break;
  default:
    assert(false);
    break;
  }

  history_.hand_action_[street_].push_back(action);
  FILE_LOG(logDEBUG3) << "Current agent in seat " << acting_player_seat_;
  acting_player_seat_ = getNextLivePlayerSeat(acting_player_seat_);
  FILE_LOG(logDEBUG2) << "Next agent in seat " << acting_player_seat_;
  updateLegalActions();  // todo: figure out if this should be in setup or end
  updateView();
  event_manager_.firePlayerActionEvent(action);

  FILE_LOG(logDEBUG3) << num_callers_ << " callers";
  FILE_LOG(logDEBUG3) << history_.hand_action_[street_].size() \
                      << " actions for street " << street_;

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
  FILE_LOG(logDEBUG3) << "getNextLivePlayerSeat " << seat;
  size_t next_seat = (seat + 1) % MAX_NUM_PLAYERS;
  while (!live_players_.count(next_seat)) {
    next_seat = (next_seat + 1) % MAX_NUM_PLAYERS;
  }
  FILE_LOG(logDEBUG3) << "getNextLivePLayerSeat return " << next_seat;
  return next_seat;
}

// add player chips to the pot
void
Game::playerBet(Player *player, uint32_t chips) {
  assert(player->chips_ >= chips);  // reaaaally don't wanna underflow
  assert(live_players_[acting_player_seat_] == player);

  FILE_LOG(logDEBUG3) << player->name_ << " bet " << chips;
  
  // player all in
  if (player->chips_ == chips) {
    allin_players_[player->getSeat()] = player;
  }

  player->chips_ -= chips;
  player->chips_in_play_ += chips;
  player_chips_in_pot_per_hand_[player->getSeat()] += chips;
  pot_ += chips;

  updateView();
  
  FILE_LOG(logDEBUG1) << "pot: " << pot_;
  FILE_LOG(logDEBUG3) << player->name_ << " has " << player->chips_in_play_ \
                      << " chips in play";
}
  
// return true if the type of action was changed
// the amount may be changed if the type is CALL or RAISE to match
// source's chips; false returned in this case
bool
Game::forceLegalAction(Action *action, Player *source) {
  assert(source != nullptr);
  FILE_LOG(logDEBUG3) << "Force legal action, type: " << action->getType();
  if (!isLegalAction(*action)) {
    FILE_LOG(logDEBUG1) << *action << " is illegal";
    if (legal_actions_[CHECK]) {
      *action = Action(CHECK, 0, source);
    } else {
      assert(legal_actions_[FOLD]);
      *action = Action(FOLD, 0, source);
    }
    return true;
  } else {
    // make sure call chips are correct
    if (action->getType() == CALL) {
      *action = Action(CALL,
                       std::min(current_bet_ - source->chips_in_play_,
                                source->chips_),
                       source);
      FILE_LOG(logDEBUG3) << "New call amount: " << action->getAmount();
    } else if (action->getType() == RAISE) {
      uint32_t min_raise = current_bet_ + current_raise_by_;
      FILE_LOG(logDEBUG3) << "force legal raise, min: " << min_raise;
      // player can't raise more their chip count
      if (action->getAmount() > source->chips_ + source->chips_in_play_) {
        *action = Action(RAISE, source->chips_ + source->chips_in_play_,
                         source);
        FILE_LOG(logDEBUG3) << "New raise amount: " << action->getAmount();
      }
      // force too low raises to either raise the min or go all in
      if (action->getAmount() < min_raise) {
        FILE_LOG(logDEBUG3) << "raise less than min: " << action->getAmount();
        if (min_raise <= source->getChips() + source->getChipsInPlay()) {
          *action = Action(RAISE, min_raise, source);
          FILE_LOG(logDEBUG3) << "forcing min raise: " << action->getAmount();
        } else {
          // all in without enough for min raise
          *action = Action(RAISE, source->chips_ + source->chips_in_play_,
                           source);
          FILE_LOG(logDEBUG3) << "forcing all in raise: " << action->getAmount();
        }
      }
    }
    FILE_LOG(logDEBUG3) << "Action is legal";
  return false;
  }
}

// todo: take ACTION_T instead
bool
Game::isLegalAction(const Action &action) {
  return legal_actions_[action.getType()];
}

// must have acting_player_seat_ pointing to the player about to act
void
Game::updateLegalActions() {
  // NUM_ACTIONS is always invalid - don't need to reset this every time
  // but I wanna constrain updating this var to this method
  legal_actions_[NUM_ACTIONS] = false;
  
  const Player &player = players_[acting_player_seat_];

  FILE_LOG(logDEBUG3) << "Updating legal actions for " << player.name_;

  bool can_raise = current_bet_ - player.getChipsInPlay() < player.getChips();

  FILE_LOG(logDEBUG3) << "Can raise: " << can_raise;

  // A post must be the first or second action of the hand
  // for now ignore the case where a blind puts player all in
  if (street_ == PREFLOP && history_.hand_action_[PREFLOP].size() < 2) {
    legal_actions_[RAISE] = false;
    legal_actions_[CALL] = false;
    legal_actions_[FOLD] = false;
    legal_actions_[CHECK] = false;
    legal_actions_[POST] = true;

    FILE_LOG(logDEBUG3) << "Allow POST";
  } else {
    if (history_.hand_action_[street_].empty()) {
      legal_actions_[RAISE] = can_raise;
      legal_actions_[CALL] = false;
      legal_actions_[FOLD] = false;
      legal_actions_[CHECK] = true;
      legal_actions_[POST] = false;

      FILE_LOG(logDEBUG3) << "Allow CHECK";
    } else {
      Action &last_action = history_.hand_action_[street_].back();
      assert(last_action.getType() < NUM_ACTIONS);

      if (last_action.getType() == CHECK) {
        legal_actions_[RAISE] = can_raise;
        legal_actions_[CALL] = false;
        legal_actions_[FOLD] = false;
        legal_actions_[CHECK] = true;
        legal_actions_[POST] = false;

        FILE_LOG(logDEBUG3) << "Allow CHECK";
      } else {
        // action.getType() == RAISE, CALL, FOLD, or POST
        legal_actions_[RAISE] = can_raise;
        legal_actions_[CALL] = true;
        legal_actions_[FOLD] = true;
        legal_actions_[CHECK] = false;
        legal_actions_[POST] = false;

        FILE_LOG(logDEBUG3) << "Allow FOLD";
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

        FILE_LOG(logDEBUG3) << "Allow CHECK";
      }
    }
  }
}

void
Game::showdown() {
  // show hole cards of all remaining live players
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    event_manager_.fireShowCardsEvent(hole_cards_[it->first], *(it->second));
  }
  
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

  history_.winner_ = players_[winning_player_seat];
  history_.known_hands_ = live_player_hands;

  std::map<size_t, Hand> winning_hands = getBestHands(live_player_hands);
  size_t num_winners = winning_hands.size();
  // round down on purpose, just throw away an extra chip if necessary
  uint32_t pot = pot_ / num_winners;

  if (num_winners > 1) {
    FILE_LOG(logDEBUG2) << "chop pot " << pot_ << ", " << pot \
                        << " between " << num_winners << " players";
  }
  for (auto it = winning_hands.begin(); it != winning_hands.end(); ++it) {
    // also updates history_.player_winnings_
    showdownWin(it->second, pot, &players_[it->first]);
  }
}

// Enforce that a player can only win up to what they have contributed to the
// pot from each other player. Pay out to the best hands until the entire
// pot is used up. For now, this doesn't properly update the hand history, TODO
void
Game::showdownAllIn() {
  FILE_LOG(logDEBUG2) << "All in showdown";

  std::map<size_t, Hand> live_player_hands = getPlayerHands();
  std::map<size_t, Hand> remaining_player_hands = live_player_hands;
  std::map<size_t, uint32_t> player_contrib = player_chips_in_pot_per_hand_;
  std::map<size_t, uint32_t> player_winnings;

  // Treat each player all in for a different amount as a side pot.
  // Each side pot is equal to the all in amount minus contributions
  // to any other smaller side pots, times the number of players in
  // that pot. Start with the smallest pot, subtract from
  // each player's contributions, and continue until all pots have
  // been considered.
  uint32_t contrib[MAX_NUM_PLAYERS];
  size_t i = 0;
  size_t num_players = player_contrib.size();  // allow duplicate pots for now
  assert(num_players <= MAX_NUM_PLAYERS);
  for (auto const &kv : player_contrib) {
    size_t seat = kv.first;
    uint32_t total_contrib_for_hand = kv.second;
    contrib[i++] = total_contrib_for_hand;
    player_winnings[seat] = 0;
  }
  std::sort(contrib, contrib + num_players);

  for (i = 0; i < num_players; i++) {
    if (contrib[i] == 0) {
      // duplicate entry in contrib
      continue;
    }

    // sum the total for this side pot and deduct from player_contrib
    uint32_t total_pot = 0;
    for (auto &kv : player_contrib) {
      size_t seat = kv.first;
      uint32_t contrib_left = kv.second;
      if (contrib_left >= contrib[i]) {
        player_contrib[seat] -= contrib[i];
        total_pot += contrib[i];
      } else {
        assert(contrib_left == 0);
        remaining_player_hands.erase(seat);
      }
    }

    FILE_LOG(logDEBUG2) << "pot bet " << contrib[i] << " with " \
                        << remaining_player_hands.size() << " players";
    FILE_LOG(logDEBUG2) << "total pot: " << total_pot;

    // deduct from later contribs
    for (size_t j = i + 1; j < num_players; j++) {
      assert(contrib[j] >= contrib[i]);
      contrib[j] -= contrib[i];
    }

    assert(remaining_player_hands.size() > 0);
    std::map<size_t, Hand> winning_hands = getBestHands(remaining_player_hands);
    size_t num_winners = winning_hands.size();
    assert(num_winners > 0);
    // round down on purpose, just throw away an extra chip if necessary
    uint32_t chop_pot = total_pot / num_winners;
    if (num_winners > 1) {
      FILE_LOG(logDEBUG3) << "chop pot, random seed: " << Random::getSeed();
    }
    for (auto const &kv : winning_hands) {
      size_t seat = kv.first;
      const Hand &hand = kv.second;

      FILE_LOG(logDEBUG2) << players_[seat].name_ \
                          << " has winning hand " \
                          << remaining_player_hands[seat].str();
      player_winnings[seat] += chop_pot;
    }
  }

  // finally give out the chips
  for (auto const &kv : player_winnings) {
    size_t seat = kv.first;
    uint32_t pot = kv.second;
    if (pot > 0) {
      showdownWin(live_player_hands[seat], pot, &players_[seat]);
    }
  }
}

void 
Game::showdownWin(const Hand &hand, uint32_t pot, Player *player) {
  event_manager_.fireShowdownEvent(hand, *player);
  FILE_LOG(logDEBUG1) << player->name_ << " wins with " << hand.str();
  potWin(pot, player);
}

void
Game::potWin(uint32_t pot, Player *player) {
  player->chips_ += pot;
  history_.winner_ = *player;
  if (history_.player_winnings_.count(player->seat_)) {
    history_.player_winnings_[player->seat_] += pot;
  } else {
    history_.player_winnings_[player->seat_] = pot;
  }
  updateView();
  event_manager_.firePotWinEvent(pot, *player);
  FILE_LOG(logDEBUG1) << player->name_ << " wins " << pot;
}

std::map<size_t, Hand>
Game::getPlayerHands() {
  std::map<size_t, Hand> player_hands;
  std::vector<Card> player_cards = board_;
  player_cards.resize(board_.size() + 2);  // make space for 2 hole cards
  for (auto it = live_players_.begin(); it != live_players_.end(); ++it) {
    // add player's first hole card
    player_cards[board_.size()] = hole_cards_[it->first].first;
    // second hole card
    player_cards[board_.size() + 1] = hole_cards_[it->first].second;
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

std::map<size_t, Hand>
Game::getBestHands(std::map<size_t, Hand> live_hands) {
  std::map<size_t, Hand> best_hands;
  int best_score = -1;
  for (auto it = live_hands.begin(); it != live_hands.end(); ++it) {
    if (it->second.eval() == best_score) {
    } else if (it->second.eval() > best_score) {
      best_score = it->second.eval();
    }
  }

  for (auto it = live_hands.begin(); it != live_hands.end(); ++it) {
    if (it->second.eval() == best_score) {
      best_hands[it->first] = it->second;
    }
  }

  return best_hands;
}

// The game is over if only one player remains with nonzero chip count
// As soon as a player loses all chips, they are removed from the Game
// so the number of players with nonzero chip counts is just the number
// of players
bool
Game::isGameOver() {
  return players_.size() == 1;
}
