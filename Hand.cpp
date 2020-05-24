#include "Hand.h"

#include "Deck.h"

void Hand::dealCard() {
  addCard(Deck::deck.getCard());
}

void Hand::addCard(Card card) {
  cards.push_back(card);
  if(totalValue() > 21) {
    for(Card & c : cards) {
      if(c.isSoft()) {
        c = Card(1);
        break;
      }
    }
  }
}

int Hand::totalValue() const {
  int tot = 0;
  for(Card c : cards) {
    tot += c.getValue();
  }
  return tot;
}

bool Hand::isSoft() const {
  for(Card c : cards) {
    if(c.isSoft()) {
      return true;
    }
  }
  return false;
}

std::ostream & operator<< (std::ostream & os, const Hand & hand) {
  const auto & cards = hand.getCards();
  os << "[ ";
  for(Card c : cards) {
    os << c.getValue() << ' ';
  }
  os << "] = " << hand.totalValue();
  return os;
}
