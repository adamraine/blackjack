#include "Player.h"

#include <iostream>
#include "Deck.h"
#include "Table.h"
#include "Count.h"
#include "Utils.h"

void Player::registerWin(const Hand & hand) {
  VPRINT << name << " wins " << hand
    << " : +" << hand.getBet() << std::endl;
  net_gains += hand.getBet();
  winAdjustBet();
}

void Player::registerBlackjack(const Hand & hand) {
  int payout = hand.getBet() + hand.getBet() / 2;
  VPRINT << name << " blackjack " << hand
    << " : +" << payout << std::endl;
  net_gains += payout;
  blackjackAdjustBet();
}

void Player::registerLoss(const Hand & hand) {
  VPRINT << name << " loses " << hand
    << " : -" << hand.getBet() << std::endl;
  net_gains -= hand.getBet();
  lossAdjustBet();
}

void Player::registerPush(const Hand & hand) {
  VPRINT << name << " push " << hand << std::endl;
  pushAdjustBet();
}

Player::SubhandResults Player::runHand(Hand hand, Card upcard) const {
  SubhandResults subhands;
  bool run = true;
  while(run && !hand.isBust()) {
    switch(makeDecision(hand, upcard)) {
      case Decision::hit:
        hand.dealCard();
        break;
      case Decision::stand:
        run = false;
        break;
      case Decision::double_down:
        if(hand.getCards().size() != 2) {
          ERROR("Cannot double here.");
        }
        hand.doubleBet();
        hand.dealCard();
        run = false;
        break;
      case Decision::split:
        if(!hand.isPair()) {
          ERROR("Cannot split here.");
        }
        Hand a(hand.getBet());
        Hand b(hand.getBet());
        Card split_card = hand.getCards()[0];
        if(hand.isSoft()) {
          split_card = Card(11);
        }
        a.addCard(split_card);
        b.addCard(split_card);

        // Run first hand
        a.dealCard();
        if(split_card.isSoft()) {
          subhands.push_back(a);
        }
        else {
          SubhandResults a_res = runHand(a, upcard);
          subhands.insert(subhands.end(), a_res.begin(), a_res.end());
        }

        // Run second hand
        b.dealCard();
        if(split_card.isSoft()) {
          subhands.push_back(b);
        }
        else {
          SubhandResults b_res = runHand(b, upcard);
          subhands.insert(subhands.end(), b_res.begin(), b_res.end());
        }

        return subhands;
    }
  }
  subhands.push_back(std::move(hand));
  return subhands;
}

Hand runDealer(Hand hand) {
  while(hand.totalValue() < 17) {
    hand.dealCard();
  }
  return hand;
}

Decision SimplePlayer::makeDecision(const Hand & hand, Card upcard) const {
  if(hand.totalValue() < 17) {
    if(hand.isPair()) {
      return Decision::split;
    }
    if(hand.getCards().size() == 2 && hand.totalValue() >= 9 && hand.totalValue() <= 11) {
      return Decision::double_down;
    }
    return Decision::hit;
  }
  return Decision::stand;
}

Decision BasicPlayer::makeDecision(const Hand & hand, Card upcard) const {
  return Table::basic_strategy.getDecision(hand, upcard);
}

void HiLoPlayer::countCards(const std::vector<Card> & cards) {
  for(Card card : cards) {
    addToCount(Count::hilo.getCountValue(card));
  }
  double true_count = getCount() / Deck::deck.getRemainingDecks();
  VPRINT << true_count << ':' << int(true_count) << std::endl;
  setBet(int(true_count));
  if(getBet() < 1) setBet(1);
}
