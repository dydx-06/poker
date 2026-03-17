#pragma once
#include <vector>
#include "card.h"

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck();
    void reset();
    void shuffle();
    Card deal();
    int remainingCards() const;
};