#include <iostream>
#include <algorithm>
#include "Random.h"
#include "Action.h"
#include "RandomAgent.h"

void
RandomAgent::receiveHoleCards(const std::pair<Card, Card> hc)
{
}

Action
RandomAgent::act(const GameView &view)
{
  // generate a random action type until it's legal
  // possibly never halts....
  const bool *legal_actions = view.getLegalActions();
  uint8_t action_type;
  do {
    action_type = Random::getInRange(0, NUM_ACTIONS - 1);
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
RandomAgent::receiveHandHistory(const HandHistory &history)
{
}
