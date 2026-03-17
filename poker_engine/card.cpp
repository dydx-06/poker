#include "card.h"

Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

std::string Card::toString() const {
    std::string rankStr;
    switch (rank) {
        case Rank::Jack: rankStr = "J"; break;
        case Rank::Queen: rankStr = "Q"; break;
        case Rank::King: rankStr = "K"; break;
        case Rank::Ace: rankStr = "A"; break;
        case Rank::Ten: rankStr = "T"; break;
        default: rankStr = std::to_string(static_cast<int>(rank)); break;
    }

    std::string suitStr;
    switch (suit) {
        case Suit::Hearts: suitStr = "h"; break;
        case Suit::Diamonds: suitStr = "d"; break;
        case Suit::Clubs: suitStr = "c"; break;
        case Suit::Spades: suitStr = "s"; break;
    }

    return rankStr + suitStr;
}