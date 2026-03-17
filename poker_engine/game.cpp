#include "game.h"
#include <iostream>

Game::Game(int startingChips) {
    players.resize(2);
    players[0].chips = startingChips;
    players[1].chips = startingChips;
    reset();
}

void Game::reset() {
    deck.reset();
    deck.shuffle();
    communityCards.clear();
    pot = 0;
    street = 0;
    
    for (auto& player : players) {
        player.reset();
        player.holeCards.push_back(deck.deal());
        player.holeCards.push_back(deck.deal());
    }

    // Implement variable blinds and preflop actions later.
    players[0].chips -= 5;
    players[0].currentBet = 5;
    players[1].chips -= 10;
    players[1].currentBet = 10;
    pot = 15;
    currentToCall = 10;
    
    activePlayer = 0;
    lastAggressor = 1; 
}

void Game::dealNextStreet() {
    street++;
    currentToCall = 0;
    players[0].currentBet = 0;
    players[1].currentBet = 0;
    activePlayer = 0;
    lastAggressor = -1;

    if (street == 1) {
        for (int i = 0; i < 3; ++i) communityCards.push_back(deck.deal());
    } else if (street == 2 || street == 3) {
        communityCards.push_back(deck.deal());
    }
}

bool Game::step(int actionIndex) {
    Action action = static_cast<Action>(actionIndex);
    Player& current = players[activePlayer];
    Player& opponent = players[1 - activePlayer];

    if (action == Action::Fold) {
        current.hasFolded = true;
        opponent.chips += pot;
        return true;
    } 
    else if (action == Action::Call) {
        int callAmount = currentToCall - current.currentBet;
        current.chips -= callAmount;
        current.currentBet += callAmount;
        pot += callAmount;
        
        if (current.currentBet == opponent.currentBet && lastAggressor != activePlayer) {
            if (street == 3) {
                processShowdown();
                return true;
            } else {
                dealNextStreet();
                return false;
            }
        }
    } 
    else if (action == Action::Raise) {
        int raiseAmount = 10; // Implement variable raise amounts later
        int totalAdd = (currentToCall - current.currentBet) + raiseAmount;
        current.chips -= totalAdd;
        current.currentBet += totalAdd;
        pot += totalAdd;
        currentToCall = current.currentBet;
        lastAggressor = activePlayer;
    }

    activePlayer = 1 - activePlayer;
    return false;
}

void Game::processShowdown() {
    HandValue p0Hand = Evaluator::evaluate(players[0].holeCards, communityCards);
    HandValue p1Hand = Evaluator::evaluate(players[1].holeCards, communityCards);

    if (p0Hand > p1Hand) {
        players[0].chips += pot;
    } else if (p1Hand > p0Hand) {
        players[1].chips += pot;
    } else {
        players[0].chips += pot / 2;
        players[1].chips += pot / 2;
    }
}
