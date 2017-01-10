#ifndef HAND_EVALUATOR_H_
#define HAND_EVALUATOR_H_

#include <cinttypes>

/* For more information on poker hands, see
 * https://en.wikipedia.org/wiki/List_of_poker_hands
 *
 * HandEvaluator uses Ray Wotton's lookup table for speedy evals. Credits
 * go to him and the rest of twoplustwo. For reference, see
 * http://archives1.twoplustwo.com/showflat.php?Cat=0&Number=8513906&page=0&fpart=13&vc=1
 * A copy of HandRanks.dat is included in the resources/ directory. The
 * code to generate this file is not included here, but can be 
 * found at the above link.
 * If the HandEvaluator test fails without modification, you should 
 * verify the following match your copy of HandRanks.dat
 * Size: 129951336 bytes
 * SHA 256: ad00f3976ad278f2cfd8c47b008cf4dbdefac642d70755a9f20707f8bbeb3c7e
 */

#define HAND_RANK_LOOKUP_FILE "resources/HandRanks.dat"

class HandEvaluator {
public:
  // Read the lookup file into memory. The file is very large - 124 megabytes,
  // so this is an expensive operation. You should avoid constructing
  // multiple copies of HandEvaluator objects. Instead, you can use
  // the public static copy in the Hand class, via Hand::evaluator_.
  HandEvaluator();

  // Frees the memory yada yada
  ~HandEvaluator();

  // Evaluate a 5 card hand. Each card should be within the range [1, 52]
  // inclusive.
  //
  // 2c = 1, 2d = 2, 2h = 3, 2s = 4, 3c = 1, ... , As = 52.
  //
  // This is the format Ray's algorithm (and the rest of 2+2  uses). To
  // convert a Card object to this integer format, use the UINT_CARD(Card)
  // wrapper macro in Hand.h.
  //
  // Return an int score of the hand. A higher score is a better hand.
  int eval5(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) const;

  // Evaluate a 7 card hand. The best 5 card combination is used to
  // determine the score. Unfortunately, there is no way to extract which
  // cards were the 5 best, so you would have to enumerate over all 21
  // combinations of hands (7 choose 5) and remember the best one.
  int eval7(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4,
            uint8_t c5, uint8_t c6) const;
private:
  int *HR_;
};

#endif  // HAND_EVALUATOR_H_
