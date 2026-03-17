#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "card.h"
#include "deck.h"
#include "evaluator.h"

int main() {
    const int ITERATIONS = 1000000;
    std::map<HandRank, int> results;

    Deck deck;

    for (int i = 0; i < ITERATIONS; ++i) {
        deck.reset();
        deck.shuffle();

        std::vector<Card> holeCards;
        holeCards.push_back(deck.deal());
        holeCards.push_back(deck.deal());

        std::vector<Card> communityCards;
        for (int j = 0; j < 5; ++j) {
            communityCards.push_back(deck.deal());
        }

        HandValue bestHand = Evaluator::evaluate(holeCards, communityCards);
        results[bestHand.rank]++;
    }

    std::cout << "Hand,Frequency\n";
    std::cout << "High Card," << results[HandRank::HighCard] << "\n";
    std::cout << "Pair," << results[HandRank::Pair] << "\n";
    std::cout << "Two Pair," << results[HandRank::TwoPair] << "\n";
    std::cout << "Three of a Kind," << results[HandRank::ThreeOfAKind] << "\n";
    std::cout << "Straight," << results[HandRank::Straight] << "\n";
    std::cout << "Flush," << results[HandRank::Flush] << "\n";
    std::cout << "Full House," << results[HandRank::FullHouse] << "\n";
    std::cout << "Four of a Kind," << results[HandRank::FourOfAKind] << "\n";
    std::cout << "Straight Flush," << results[HandRank::StraightFlush] << "\n";
    std::cout << "Royal Flush," << results[HandRank::RoyalFlush] << "\n";

    return 0;
}