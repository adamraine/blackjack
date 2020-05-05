#include <iostream>
#include <fstream>
#include <cassert>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Utils.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

vector<unique_ptr<Player> > loadPlayers(string filename) {
    vector<unique_ptr<Player> > players;
    if(filename.empty()) {
        players.push_back(make_unique<SimplePlayer>("Player", 1));
        return players;
    }
    std::fstream fin(filename);
    string name, type, bet;
    while(fin >> name >> type >> bet) {
        if(type == "simple") {
            players.push_back(make_unique<SimplePlayer>(name, stoi(bet)));
        }
        else if(type == "basic") {
            players.push_back(make_unique<BasicPlayer>(name, stoi(bet)));
        }
        else if(type == "hilo") {
            players.push_back(make_unique<HiLoPlayer>(name, stoi(bet)));
        }
        else {
            ERROR("Invalid player type: " + type);
        }
    }
    VPRINT << players.size() << std::endl;
    return players;
}

int main(int argc, char* argv[]) {

    Options::getOptions(argc, argv);

    Deck::deck = Deck(Options::num_decks, Options::deck_seed);

    vector<unique_ptr<Player> > players = loadPlayers(Options::players);
    vector<Hand> hands;

    for(int r = 0; r < Options::num_rounds; ++r) {
        VPRINT << "#### Round: " << r << " ####" << std::endl;

        Hand dealer(0);

        hands.clear();
        for(const auto & player : players) {
            hands.push_back(Hand(player->getBet()));
        }

        // First round
        for(Hand & hand : hands) {
            hand.dealCard();
        }
        Card upcard = Deck::deck.getCard();
        dealer.addCard(upcard);

        // Second round
        for(Hand & hand : hands) {
            hand.dealCard();
        }
        dealer.dealCard();

        bool still_alive = false;
        vector<Player::SubhandResults> results;

        // Run player hands
        for(size_t p = 0; p < players.size(); ++p) {
            const Hand & hand = hands[p];
            Player & player = *players[p];
            Player::SubhandResults subhands;
            if(hand.isBlackjack() && !dealer.isBlackjack()) {
                if(player.takeEvenMoney()) {
                    player.registerWin(hand);
                }
                else {
                    player.registerBlackjack(hand);
                }
            }
            else if(!hand.isBlackjack() && dealer.isBlackjack()) {
                player.registerLoss(hand);
            }
            else if(!hand.isBlackjack() && !dealer.isBlackjack()) {
                subhands = player.runHand(hand, upcard);

                // Check for player busts before running dealer hand
                for(const Hand & sub : subhands) {
                    if(sub.isBust()) {
                        player.registerLoss(sub);
                    }
                    else {
                        still_alive = true;
                    }
                }
            }
            else {
                if(player.takeEvenMoney()) {
                    player.registerWin(hand);
                }
                else {
                    player.registerPush(hand);
                }
            }
            results.push_back(subhands);
        }

        // Don't run dealer's hand if all players bust
        if(still_alive) {
            dealer = runDealer(dealer);
            for(size_t p = 0; p < players.size(); ++p) {
                Player & player = *players[p];
                const Player::SubhandResults & subhands = results[p];
                if(subhands.empty()) continue;
                for(const Hand & sub : subhands) {
                    if(sub.isBust()) {
                        continue;
                    }
                    else if(dealer.isBust()) {
                        player.registerWin(sub);
                    }
                    else if(sub.totalValue() < dealer.totalValue()) {
                        player.registerLoss(sub);
                    }
                    else if(sub.totalValue() > dealer.totalValue()) {
                        player.registerWin(sub);
                    }
                    else {
                        player.registerPush(sub);
                    }
                }
            }
        }

        // Perform card counting
        for(auto & pl : players) {
            for(const Player::SubhandResults & subhands : results) {
                for(const Hand & hand : subhands) {
                    pl->countCards(hand.getCards());
                }
            }
            pl->countCards(dealer.getCards());
        }

        VPRINT << "- Dealer hand: " << dealer << " -" << std::endl;

        if(Deck::deck.shouldReset()) {
            Deck::deck.reset();
            for(auto & pl : players) {
                pl->reset();
            }
        }
    }

    for(auto & pl : players) {
        std::cout << "#### " << pl->getName() << " net gains: " << pl->getNetGains() << " ####" << std::endl;
    }
}
