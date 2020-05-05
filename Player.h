#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <memory>
#include "Hand.h"
#include "Card.h"

enum Decision {
    stand,
    hit,
    double_down,
    split
};

class Player {
private:
    std::string name;
    double count;
    int bet;
    int net_gains;
protected:
    void addToCount(double to_add) { count += to_add; }
public:
    Player(std::string _name, int _bet) : name(_name), count(0), bet(_bet), net_gains(0) {}

    std::string getName() { return name; }
    double getCount() { return count; }
    int getBet() { return bet; }
    void setBet(int _bet) { bet = _bet; }
    int getNetGains() { return net_gains; }

    void reset() { count = 0; bet = 1; }

    void registerWin(const Hand & hand);
    void registerBlackjack(const Hand & hand);
    void registerLoss(const Hand & hand);
    void registerPush(const Hand & hand);

    virtual void winAdjustBet() {};
    virtual void blackjackAdjustBet() {}
    virtual void lossAdjustBet() {};
    virtual void pushAdjustBet() {};

    virtual bool takeEvenMoney() { return false; }
    virtual int getInsuranceBet() { return 0; }

    /* If a hand splits we must check more than one result */
    using SubhandResults = std::vector<Hand>;
    SubhandResults runHand(Hand hand, Card upcard) const;

    virtual Decision makeDecision(const Hand & hand, Card upcard) const = 0;

    virtual void countCards(const std::vector<Card> & cards) = 0;
};

Hand runDealer(Hand hand);

class SimplePlayer : public Player {
public:
    SimplePlayer(std::string _name, int _bet) : Player(_name, _bet) {}
    Decision makeDecision(const Hand & hand, Card upcard) const;
    void countCards(const std::vector<Card> & cards) {}
};

class BasicPlayer : public Player {
public:
    BasicPlayer(std::string _name, int _bet) : Player(_name, _bet) {}
    Decision makeDecision(const Hand & hand, Card upcard) const;
    virtual void countCards(const std::vector<Card> & cards) {}
};

class HiLoPlayer : public BasicPlayer {
public:
    HiLoPlayer(std::string _name, int _bet) : BasicPlayer(_name, _bet) {}
    void countCards(const std::vector<Card> & cards);
};

#endif // PLAYER_H_