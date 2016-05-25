#include <cinttypes>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "Card.h"
#include "Deck.h"


Deck::Deck() {
  constructDeck();
  shuffle();
}

// Uses the modern Fisher-Yates shuffle
void Deck::shuffle() {
  constructDeck();
  for (size_t i = deck_.size() - 1; i > 0; i--) {
    size_t j = rand() % (i + 1);
    Card temp = deck_[i];
    deck_[i] = deck_[j];
    deck_[j] = temp;
  }
}
const Card Deck::dealNextCard() {
  Card c = deck_.back();
  deck_.pop_back();
  return c;
}

const std::pair<Card, Card> Deck::dealHoleCards() {
  return std::pair<Card, Card>(dealNextCard(), dealNextCard());
}

void Deck::constructDeck() {
  deck_.clear();
  for (uint8_t rank = TWO; rank <= ACE; rank++) {
    // unrolled here because suits don't have an intrinsic order
    deck_.push_back(Card(rank, CLUBS));
    deck_.push_back(Card(rank, HEARTS));
    deck_.push_back(Card(rank, SPADES));
    deck_.push_back(Card(rank, DIAMONDS));
  }
}
