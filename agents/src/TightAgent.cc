#include <algorithm>
#include "TightAgent.h"

void
TightAgent::receiveHoleCards(const std::pair<Card, Card> hc) {
  hc_ = hc;
}

// raise if hc_ > 22 or > KQ
Action
TightAgent::act(const GameView &view) {
  uint8_t rank0 = hc_.first.rank();
  uint8_t rank1 = hc_.second.rank();
  int min_raise = view.getCurrentBet() + view.getCurrentRaiseBy();
  int max_raise = self_->getChips() + self_->getChipsInPlay();
  int raise_amount = std::min(max_raise, min_raise + 10 * rank0);
  bool can_raise = view.getLegalActions()[RAISE];

  
  if (hc_.first.rank() == hc_.second.rank() ||
      rank0 + rank1 > RANK(KING) + RANK(QUEEN)) {

    if (can_raise) {
      return Action(RAISE, min_raise + (max_raise - min_raise) / 4);
    } else {
      return Action(CALL);
    }
  }
  
  return Action(FOLD);
}

void
TightAgent::receiveHandHistory(const HandHistory &history) {

}
