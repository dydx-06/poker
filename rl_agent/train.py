import os
import numpy as np
import torch
from poker_env_wrapper import TexasHoldemEnv
from dqn_agent import DQNAgent

NUM_EPISODES = 50000
BATCH_SIZE = 128
EPSILON_START = 0.3
EPSILON_END = 0.02
EPSILON_DECAY = 0.99995
TARGET_UPDATE_FREQ = 200

def train():
    env = TexasHoldemEnv(starting_chips=1000)
    agent = DQNAgent(state_dim=18, action_dim=3)
    
    if os.path.exists("brain.pth"):
        print("--- Using existing brain ---")
        agent.q_network.load_state_dict(torch.load("brain.pth"))
        agent.update_target_network()

    epsilon = EPSILON_START
    total_rewards = []

    for episode in range(1, NUM_EPISODES + 1):
        state, _ = env.reset()
        episode_reward = 0
        terminated = False
        
        while not terminated:
            action = agent.act(state, epsilon)
            next_state, reward, terminated, _, _ = env.step(action)
            agent.memory.push(state, action, reward, next_state, terminated)
            state = next_state
            episode_reward += reward
            agent.train(BATCH_SIZE)

        if episode % TARGET_UPDATE_FREQ == 0:
            agent.update_target_network()

        epsilon = max(EPSILON_END, epsilon * EPSILON_DECAY)
        total_rewards.append(episode_reward)

        if episode % 500 == 0:
            avg_reward = np.mean(total_rewards[-500:])
            print(f"Hand: {episode} | Avg Reward (Last 500): {avg_reward:.2f} | Epsilon: {epsilon:.3f}")
            torch.save(agent.q_network.state_dict(), "brain.pth")

    print("\nGreat success")

if __name__ == "__main__":
    train()