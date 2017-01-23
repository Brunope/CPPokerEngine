#include "GameView.h"
#include "Action.h"
#include "HandHistory.h"
#include "SimpleAgent.h"

Action
SimpleAgent::act(const GameView &gameView) {
  double num_bb =
    static_cast<double>(self_->getChips()) / gameView.getBigBlind();
  size_t call_amt = gameView.getCurrentBet() - self_->getChipsInPlay();
  
  if (gameView.getStreet() == PREFLOP) {
    if (hc_.first.rank() >= TEN && hc_.second.rank() >= TEN) {
      return Action(RAISE, gameView.getCurrentBet() * 2);
    } else {
      return Action(FOLD);
    }
  }
  return Action(FOLD);
}

void
SimpleAgent::receiveHoleCards(const std::pair<Card, Card> hc) {
  hc_ = hc;
}

void
SimpleAgent::receiveHandHistory(const HandHistory &history) {

}
