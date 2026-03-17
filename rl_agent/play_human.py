import torch
import numpy as np
from poker_env_wrapper import TexasHoldemEnv
from dqn_agent import PokerDQN

def main():
    print("--- Poker? I barely know 'er! ---")
    
    env = TexasHoldemEnv(starting_chips=1000)
    state_dim = 18
    action_dim = 3
    
    model = PokerDQN(state_dim, action_dim)
    try:
        model.load_state_dict(torch.load("brain.pth"))
        model.eval()
        print("Brain loaded.")
    except FileNotFoundError:
        print("Brain not found.")
        return

    action_map = {0: "Fold", 1: "Call/Check", 2: "Raise"}

    rank_map = {
        2: '2', 3: '3', 4: '4', 5: '5', 6: '6', 7: '7', 8: '8', 
        9: '9', 10: 'T', 11: 'J', 12: 'Q', 13: 'K', 14: 'A'
    }
    
    suit_map = {
        0: 'h',
        1: 'd',
        2: 'c',
        3: 's'
    }

    def get_card_str(rank_float, suit_float):
        r = rank_map.get(int(rank_float), str(int(rank_float)))
        s = suit_map.get(int(suit_float), f"Suit({int(suit_float)})")
        return f"{r}{s}"

    while True:
        state, info = env.reset()
        terminated = False
        
        print("\n" + "="*40)
        print("          NEW HAND DEALT")
        print("="*40)

        while not terminated:
            active_player = info.get("current_player", 0)

            if active_player == 0:
                card1 = get_card_str(state[0], state[1])
                card2 = get_card_str(state[2], state[3])
                
                board = []
                for i in range(5):
                    r_idx = 4 + (i * 2)
                    s_idx = 5 + (i * 2)
                    if state[r_idx] > 0 or (state[r_idx] == 0 and state[s_idx] > 0): 
                        board.append(get_card_str(state[r_idx], state[s_idx]))

                board_str = " ".join(board) if board else "[Pre-flop]"

                print(f"\n[Your Turn]")
                print(f"Your Cards: {card1}  {card2}")
                print(f"Board:      {board_str}")
                print(f"Pot:        {state[14]} | Your Chips: {state[16]}")
                
                valid_input = False
                while not valid_input:
                    move = input("Enter Action (0: Fold, 1: Call/Check, 2: Raise): ")
                    if move in ['0', '1', '2']:
                        action = int(move)
                        valid_input = True
                    else:
                        print("Invalid input. Please enter 0, 1, or 2.")
            
            else:
                print("\n[AI's Turn]")
                state_tensor = torch.FloatTensor(state).unsqueeze(0)
                
                with torch.no_grad():
                    q_values = model(state_tensor)
                    action = torch.argmax(q_values).item()
                
                print(f"AI Action: {action_map[action]}")

            state, reward, terminated, truncated, info = env.step(action)

        print("\n" + "-"*40)
        print("HAND COMPLETE")
        
        try:
            ai_player = env.game.players[1]
            if len(ai_player.holeCards) == 2:
                ai_c1 = get_card_str(ai_player.holeCards[0].rank, ai_player.holeCards[0].suit)
                ai_c2 = get_card_str(ai_player.holeCards[1].rank, ai_player.holeCards[1].suit)
                print(f"AI Mucks: {ai_c1}  {ai_c2}")
        except Exception:
            print("AI Mucks: [Cards already cleared]")
        
        print(f"Final Reward signal for your seat: {reward}")
        
        play_again = input("\nPlay another hand? (y/n): ")
        if play_again.lower() != 'y':
            print("Thanks for playing!")
            break

if __name__ == "__main__":
    main()