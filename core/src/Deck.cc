#include <cinttypes>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>
#include <random>

#include "Card.h"
#include "Deck.h"

static std::random_device rd;
static std::mt19937 rng(rd());

Deck::Deck() {
  shuffle();
}

// Fisher-Yates shuffle
void
Deck::fyShuffle() {
  for (size_t i = deck_.size() - 1; i > 0; i--) {
    size_t j = rng() % (i + 1);
    Card temp = deck_[i];
    deck_[i] = deck_[j];
    deck_[j] = temp;
  }
}

void
Deck::shuffle() {
  constructDeck();
  fyShuffle();
}

void
Deck::shuffle7() {
  constructDeck();
  for (size_t i = 0; i < 7; i++) {
    fyShuffle();
  }
}

const Card
Deck::dealNextCard() {
  assert(!deck_.empty());
  Card c = deck_.back();
  deck_.pop_back();
  return c;
}

const std::pair<Card, Card>
Deck::dealHoleCards() {
  return std::pair<Card, Card>(dealNextCard(), dealNextCard());
}

uint8_t
Deck::cardsLeft() const {
  return deck_.size();
}

void
Deck::constructDeck() {
  deck_.clear();
  for (uint8_t rank = TWO; rank <= ACE; rank++) {
    // unrolled here because suits don't have an intrinsic order
    deck_.push_back(Card(rank, CLUBS));
    deck_.push_back(Card(rank, HEARTS));
    deck_.push_back(Card(rank, SPADES));
    deck_.push_back(Card(rank, DIAMONDS));
  }
}
