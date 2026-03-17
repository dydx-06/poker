#pragma once
#include <string>

enum class Suit {
    Hearts, Diamonds, Clubs, Spades
};

enum class Rank {
    Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King, Ace
};

class Card {
public:
    Rank rank;
    Suit suit;

    Card(Rank r, Suit s);
    std::string toString() const;
};