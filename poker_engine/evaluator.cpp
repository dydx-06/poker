#include "evaluator.h"
#include <algorithm>
#include <map>

HandValue Evaluator::evaluate(const std::vector<Card>& holeCards, const std::vector<Card>& communityCards) {

    std::vector<Card> allCards = holeCards;
    allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());
    std::sort(allCards.begin(), allCards.end(), [](const Card& a, const Card& b) {
        return static_cast<int>(a.rank) > static_cast<int>(b.rank);
    });

    std::map<Rank, int> rankCounts;
    std::map<Suit, int> suitCounts;
    for (const auto& card : allCards) {
        rankCounts[card.rank]++;
        suitCounts[card.suit]++;
    }

    // Straight Tracking
    bool isStraight = false;
    int straightHighRank = 0;
    std::vector<int> uniqueRanks;
    for (const auto& card : allCards) {
        int currentRank = static_cast<int>(card.rank);
        if (uniqueRanks.empty() || uniqueRanks.back() != currentRank) {
            uniqueRanks.push_back(currentRank);
        }
    }

    int consecutiveCount = 1;
    for (size_t i = 1; i < uniqueRanks.size(); ++i) {
        if (uniqueRanks[i - 1] - 1 == uniqueRanks[i]) {
            consecutiveCount++;
            if (consecutiveCount >= 5) {
                isStraight = true;
                straightHighRank = uniqueRanks[i - 4];
                break;
            }
        } else {
            consecutiveCount = 1;
        }
    }

    // Check for A-2-3-4-5 case
    if (!isStraight && uniqueRanks.front() == static_cast<int>(Rank::Ace)) {
        bool hasFive = false, hasFour = false, hasThree = false, hasTwo = false;
        for (int rank : uniqueRanks) {
            if (rank == static_cast<int>(Rank::Five)) hasFive = true;
            if (rank == static_cast<int>(Rank::Four)) hasFour = true;
            if (rank == static_cast<int>(Rank::Three)) hasThree = true;
            if (rank == static_cast<int>(Rank::Two)) hasTwo = true;
        }
        if (hasFive && hasFour && hasThree && hasTwo) {
            isStraight = true;
            straightHighRank = 5;
        }
    }

    // Flush Tracking
    bool isFlush = false;
    Suit flushSuit;
    for (const auto& pair : suitCounts) {
        if (pair.second >= 5) {
            isFlush = true;
            flushSuit = pair.first;
            break;
        }
    }

    int pairs = 0, threeOfAKind = 0, fourOfAKind = 0;
    for (const auto& pair : rankCounts) {
        if (pair.second == 2) pairs++;
        else if (pair.second == 3) threeOfAKind++;
        else if (pair.second == 4) fourOfAKind++;
    }

    // --- EVALUATION TREE ---

    // 1. Straight Flush 
    if (isFlush) {
        std::vector<int> flushRanks;
        for (const auto& card : allCards) {
            if (card.suit == flushSuit) {
                flushRanks.push_back(static_cast<int>(card.rank));
            }
        }

        bool isStraightFlush = false;
        int sfHighRank = 0;
        int sfConsecutiveCount = 1;

        for (size_t i = 1; i < flushRanks.size(); ++i) {
            if (flushRanks[i - 1] - 1 == flushRanks[i]) {
                sfConsecutiveCount++;
                if (sfConsecutiveCount >= 5) {
                    isStraightFlush = true;
                    sfHighRank = flushRanks[i - 4];
                    break;
                }
            } else {
                sfConsecutiveCount = 1;
            }
        }

        // Check the A-2-3-4-5 edge case specifically for the flush cards
        if (!isStraightFlush && flushRanks.front() == static_cast<int>(Rank::Ace)) {
            bool has5 = false, has4 = false, has3 = false, has2 = false;
            for (int r : flushRanks) {
                if (r == 5) has5 = true;
                if (r == 4) has4 = true;
                if (r == 3) has3 = true;
                if (r == 2) has2 = true;
            }
            if (has5 && has4 && has3 && has2) {
                isStraightFlush = true;
                sfHighRank = 5;
            }
        }

        if (isStraightFlush) {
            std::vector<int> sfCards;
            if (sfHighRank == 5) sfCards = {5, 4, 3, 2, 14};
            else sfCards = {sfHighRank, sfHighRank-1, sfHighRank-2, sfHighRank-3, sfHighRank-4};
            
            if (sfHighRank == 14) {
                return HandValue{HandRank::RoyalFlush, sfCards};
            }
            return HandValue{HandRank::StraightFlush, sfCards};
        }
    }

    // 2. Four of a Kind
    if (fourOfAKind > 0) {
        int quadRank = 0, kicker = 0;
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            if (it->second == 4) quadRank = static_cast<int>(it->first);
            else if (kicker == 0) kicker = static_cast<int>(it->first);
        }
        return HandValue{HandRank::FourOfAKind, std::vector<int>{quadRank, quadRank, quadRank, quadRank, kicker}};
    }

    // 3. Full House
    if ((threeOfAKind > 0 && pairs > 0) || threeOfAKind > 1) {
        int tripRank = 0, pairRank = 0;
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            if (it->second >= 3 && tripRank == 0) tripRank = static_cast<int>(it->first);
            else if (it->second >= 2 && pairRank == 0) pairRank = static_cast<int>(it->first);
        }
        return HandValue{HandRank::FullHouse, std::vector<int>{tripRank, tripRank, tripRank, pairRank, pairRank}};
    }

    // 4. Flush
    if (isFlush) {
        std::vector<int> flushCards;
        for (const auto& card : allCards) {
            if (card.suit == flushSuit && flushCards.size() < 5) {
                flushCards.push_back(static_cast<int>(card.rank));
            }
        }
        return HandValue{HandRank::Flush, flushCards};
    }

    // 5. Straight
    if (isStraight) {
        std::vector<int> stCards;
        if (straightHighRank == 5) stCards = {5, 4, 3, 2, 14};
        else stCards = {straightHighRank, straightHighRank-1, straightHighRank-2, straightHighRank-3, straightHighRank-4};
        return HandValue{HandRank::Straight, stCards};
    }

    // 6. Three of a Kind
    if (threeOfAKind > 0) {
        int tripRank = 0;
        std::vector<int> kickers;
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            if (it->second == 3 && tripRank == 0) tripRank = static_cast<int>(it->first);
            else if (kickers.size() < 2) kickers.push_back(static_cast<int>(it->first));
        }
        return HandValue{HandRank::ThreeOfAKind, std::vector<int>{tripRank, tripRank, tripRank, kickers[0], kickers[1]}};
    }

    // 7. Two Pair
    if (pairs > 1) {
        int highPairRank = 0, lowPairRank = 0, highestKicker = 0;
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            if (it->second >= 2) {
                if (highPairRank == 0) highPairRank = static_cast<int>(it->first);
                else if (lowPairRank == 0) lowPairRank = static_cast<int>(it->first);
            }
        }
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            int currentRank = static_cast<int>(it->first);
            if (currentRank != highPairRank && currentRank != lowPairRank) {
                highestKicker = currentRank;
                break;
            }
        }
        return HandValue{HandRank::TwoPair, std::vector<int>{highPairRank, highPairRank, lowPairRank, lowPairRank, highestKicker}};
    }

    // 8. Pair
    if (pairs == 1) {
        int pairRank = 0;
        std::vector<int> kickers;
        for (auto it = rankCounts.rbegin(); it != rankCounts.rend(); ++it) {
            if (it->second == 2 && pairRank == 0) pairRank = static_cast<int>(it->first);
            else if (kickers.size() < 3) kickers.push_back(static_cast<int>(it->first));
        }
        return HandValue{HandRank::Pair, std::vector<int>{pairRank, pairRank, kickers[0], kickers[1], kickers[2]}};
    }

    // 9. High Card
    return HandValue{HandRank::HighCard, std::vector<int>{
        static_cast<int>(allCards[0].rank), static_cast<int>(allCards[1].rank), 
        static_cast<int>(allCards[2].rank), static_cast<int>(allCards[3].rank), 
        static_cast<int>(allCards[4].rank)
    }};
}