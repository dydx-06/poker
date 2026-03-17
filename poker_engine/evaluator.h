#pragma once
#include <vector>
#include "card.h"

enum class HandRank {
    HighCard, 
    Pair, 
    TwoPair, 
    ThreeOfAKind,
    Straight, 
    Flush, 
    FullHouse, 
    FourOfAKind, 
    StraightFlush, 
    RoyalFlush
};

struct HandValue {
    HandRank rank;
    std::vector<int> kickers;

    bool operator>(const HandValue& other) const {
        if (rank != other.rank) {
            return static_cast<int>(rank) > static_cast<int>(other.rank);
        }
        for (size_t i = 0; i < kickers.size(); ++i) {
            if (kickers[i] != other.kickers[i]) {
                return kickers[i] > other.kickers[i];
            }
        }
        return false;
    }
};

class Evaluator {
public:
    static HandValue evaluate(const std::vector<Card>& holeCards, const std::vector<Card>& communityCards);
};