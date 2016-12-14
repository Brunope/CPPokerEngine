#include <iostream>
#include <string>
#include <cassert>

#include "Action.h"
#include "Player.h"
#include "HandHistory.h"
#include "Card.h"
#include "HumanActor.h"

void
HumanActor::receiveHoleCards(const std::pair<Card, Card> hc) {
  std::cout << "You were dealt " << hc.first << hc.second << std::endl;
}

Action
HumanActor::act(const GameView &view) {
  std::vector<Card> board = view.getBoard();
  if (board.size() > 0) {
    std::cout << "board: ";
    for (auto it = board.begin(); it != board.end(); ++it) {
      std::cout << *it;
    }
    std::cout << std::endl;
  }

  std::cout << "current bet: " << view.getCurrentBet() << std::endl;

  const bool *legal_actions = view.getLegalActions();
  for (uint8_t action_type = 0; action_type < NUM_ACTIONS; action_type++) {
    if (legal_actions[action_type]) {
      switch (action_type) {
      case RAISE:
        std::cout << "[r]aise ";
        break;
      case CALL:
        std::cout << "[c]all ";
        break;
      case FOLD:
        std::cout << "[f]old ";
        break;
      case CHECK:
        // ok to use 'c' for this too, since it's never the case that
        // both check and fold are legal actions
        std::cout << "[c]heck ";
        break;
      default:
        assert(false);
        break;
      }
    }
  }

  std::cout << std::endl << "action: ";
  char type;
  std::cin >> type;
  switch (type) {
  case 'r':
    // get amount
    uint32_t amount;
    std::cout << "amount: ";
    std::cin >> amount;

    return Action(RAISE, amount);
    break;
  case 'c':
    if (legal_actions[CALL]) {
      return Action(CALL);
    } else {
      return Action(CHECK);
    }
    break;
  case 'f':
    return Action(FOLD);
    break;
  default:
    std::cout << "bad char, you have to fold now, sorry" << std::endl;
    return Action(FOLD);
    break;
  }
}

void
HumanActor::receiveHandHistory(const HandHistory &history) {
}
