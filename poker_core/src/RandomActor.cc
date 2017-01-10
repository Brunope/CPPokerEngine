#include <iostream>
#include <random>
#include <algorithm>

#include "Action.h"
#include "RandomActor.h"

RandomActor::RandomActor()
{
  mt_ = std::mt19937(std::random_device{}());
  dist_ = std::uniform_int_distribution<int>
    (0, static_cast<int>(NUM_ACTIONS) - 1);
}

void
RandomActor::receiveHoleCards(const std::pair<Card, Card> hc)
{
}

Action
RandomActor::act(const GameView &view)
{
  // generate a random action type until it's legal
  // possibly never halts....
  const bool *legal_actions = view.getLegalActions();
  uint8_t action_type;
  do {
    action_type = dist_(mt_);
  } while (!legal_actions[action_type]);

  // If raising, just raise double what the last player raised, or what
  // we have left, whichever is smaller
  if (action_type == RAISE) {
    uint32_t raise_amt =
      std::min(2 * view.getCurrentRaiseBy() + view.getCurrentBet(),
               self_->getChips() + self_->getChipsInPlay());
    return Action(RAISE, raise_amt);
  } else {
    return Action(static_cast<ACTION_T>(action_type));
  }
}

void
RandomActor::receiveHandHistory(const HandHistory &history)
{
}
