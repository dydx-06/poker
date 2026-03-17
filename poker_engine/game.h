#ifndef GAME_H
#define GAME_H

#include <vector>
#include "card.h"
#include "deck.h"
#include "evaluator.h"

enum class Action {
    Fold = 0,
    Call = 1,
    Raise = 2
};

struct Player {
    int chips;
    int currentBet;
    bool hasFolded;
    std::vector<Card> holeCards;
    
    void reset() {
        currentBet = 0;
        hasFolded = false;
        holeCards.clear();
    }
};

class Game {
public:
    Game(int startingChips);
    
    void reset();
    bool step(int actionIndex);

    std::vector<int> getObservation(int playerIndex) const;
    int getReward(int playerIndex) const;

    Deck deck;
    std::vector<Player> players;
    std::vector<Card> communityCards;
    int pot;
    int currentToCall;
    int activePlayer;
    int street;

private:
    void dealNextStreet();
    void processShowdown();
    int lastAggressor;
};

#endif