#ifndef DECK_H_
#define DECK_H_

#include <vector>
#include "Card.h"

class Deck {
  std::vector<Card> cards;
  size_t num_decks;
  size_t seed;
public:
  Deck(size_t _num_decks);
  Deck(size_t _num_decks, size_t _seed);
  void reset();
  bool shouldReset();
  Card getCard();
  double getRemainingDecks();

  static Deck deck;
};

#endif // DECK_H_
