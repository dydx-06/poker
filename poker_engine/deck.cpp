#include "deck.h"
#include <algorithm>
#include <random>
#include <stdexcept>

Deck::Deck() {
    reset();
}

void Deck::reset() {
    cards.clear();
    for (int s = static_cast<int>(Suit::Hearts); s <= static_cast<int>(Suit::Spades); ++s) {
        for (int r = static_cast<int>(Rank::Two); r <= static_cast<int>(Rank::Ace); ++r) {
            cards.emplace_back(static_cast<Rank>(r), static_cast<Suit>(s));
        }
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::deal() {
    if (cards.empty()) {
        throw std::out_of_range("The deck is empty.");
    }
    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}

int Deck::remainingCards() const {
    return cards.size();
}