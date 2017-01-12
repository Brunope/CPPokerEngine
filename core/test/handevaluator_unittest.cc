#include "gtest/gtest.h"
#include "Hand.h"
#include "HandEvaluator.h"

// twoplustwo benchmark tests
TEST(HandEvaluatorTest, Comprehensive) {
  int handTypeSum[10];
  memset(handTypeSum, 0, sizeof(handTypeSum));
  int count = 0;
  for (int c0 = 1; c0 < 53; c0++) {
    for (int c1 = c0 + 1; c1 < 53; c1++) {
      for (int c2 = c1 + 1; c2 < 53; c2++) {
        for (int c3 = c2 + 1; c3 < 53; c3++) {
          for (int c4 = c3 + 1; c4 < 53; c4++) {
            for (int c5 = c4 + 1; c5 < 53; c5++) {
              for (int c6 = c5 + 1; c6 < 53; c6++) {
                int strength = Hand::evaluator_.eval7(c0, c1, c2,
                                                      c3, c4, c5, c6);
                EXPECT_LE(strength >> 12, 9);
                handTypeSum[strength >> 12]++;
                count++;
              }
            }
          }
        }
      }
    }
  }
  int handTypeSumExp[] = { 0, 23294460, 58627800, 31433400, 6461620,
                           6180020, 4047644, 3473184, 224848, 41584 };
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(handTypeSum[i], handTypeSumExp[i]);
  }
  EXPECT_EQ(count, 133784560);
}
