#include "GameView.h"
#include "Actions.h"
#include "SimpleActor.h"

Action
SimpleActor::act(const GameView &gameView) {
  double num_bb =
    static_cast<double>(self_.getChips()) / gameView.getBigBlind();
  size_t call_amt = gameView.getCurrentBet() - self_.getChipsInPlay();
  
  if (gameView.getStreet() == PREFLOP) {
    if (hc_.first.rank() >= TEN && hc_.second.rank() >= TEN) {
      return Raise(gameView.getCurrentBet() * 2);
    } else {
      return Fold();
    }
  }
  return Fold();
}

void SimpleActor::receiveHandHistory(const std::vector<const Action> &history) {

}
