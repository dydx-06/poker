import poker_env

def main():

    deck = poker_env.Deck()
    deck.shuffle()

    hole_cards = [deck.deal(), deck.deal()]
    community_cards = [deck.deal() for _ in range(5)]

    print("Hole Cards:")
    for card in hole_cards:
        print(f"  {card.to_string()}")

    print("\nBoard:")
    for card in community_cards:
        print(f"  {card.to_string()}")

    best_hand = poker_env.Evaluator.evaluate(hole_cards, community_cards)

    print(f"  Rank:    {best_hand.rank.name}") 
    print(f"  Kickers: {best_hand.kickers}\n")

if __name__ == "__main__":
    main()