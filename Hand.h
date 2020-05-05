#ifndef HAND_H_
#define HAND_H_

#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;
    int bet;
public:
    Hand(int _bet) : bet(_bet) {}

    void addCard(Card card);
    void dealCard();
    void doubleBet() { bet *= 2; }

    const std::vector<Card> & getCards() const { return cards; }
    int getBet() const { return bet; }

    int totalValue() const;
    bool isSoft() const;
    bool isBust() const { return totalValue() > 21; }
    bool isBlackjack() const { return cards.size() == 2 && totalValue() == 21; }
    bool isPair() const { return cards.size() == 2 && cards[0].getValue() % 10 == cards[1].getValue() % 10; };
};

std::ostream & operator<< (std::ostream & os, const Hand & hand);

#endif // HAND_H_