#include <algorithm>
#include <random>
#include <ctime>
#include "Deck.h"

Deck Deck::deck(6);

Deck::Deck(size_t _num_decks) : Deck(_num_decks, 0) {}

Deck::Deck(size_t _num_decks, size_t _seed)
  : num_decks(_num_decks), seed(_seed) {
    cards.reserve(num_decks * 52);
    reset();
  }

void Deck::reset() {
  cards.clear();
  for(size_t d = 0; d < 4 * num_decks; ++d) {
    // Add 2 through 9
    for(int v = 2; v <= 9; ++v) {
      cards.push_back(v);
    }
    // Add 4 tens
    for(int t = 0; t < 4; ++t) {
      cards.push_back(10);
    }
    cards.push_back(11);
  }
  shuffle(cards.begin(), cards.end(), std::default_random_engine(seed ? seed : time(0)));
}

bool Deck::shouldReset() {
  size_t size = 52 * num_decks;
  return size / cards.size() >= 4;
}

Card Deck::getCard() {
  Card ret = cards.back();
  cards.pop_back();
  return ret;
}

double Deck::getRemainingDecks() {
  return cards.size() / double(52);
}
