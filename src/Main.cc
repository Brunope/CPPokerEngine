#include <cstdlib>
#include <iostream>

#include "Card.h"

int main(void) {
  Card c(ACE, SPADES);

  std::cout << c << std::endl;

  return EXIT_SUCCESS;
}
