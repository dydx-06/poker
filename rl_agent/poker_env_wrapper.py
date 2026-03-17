import gymnasium as gym
from gymnasium import spaces
import numpy as np
import poker_env

class TexasHoldemEnv(gym.Env):
    def __init__(self, starting_chips=1000):
        super().__init__()
        
        self.action_space = spaces.Discrete(3)
        self.observation_space = spaces.Box(low=0, high=10000, shape=(18,), dtype=np.float32)
        
        self.game = poker_env.Game(starting_chips)

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        self.game.reset()
        
        observation = self._get_observation()
        # FIX 1: Pass the active player in the info dictionary
        info = {"current_player": self.game.activePlayer}
        return observation, info

    def step(self, action):
        acting_player_idx = self.game.activePlayer
        chips_before = self.game.players[acting_player_idx].chips

        terminated = self.game.step(action)

        chips_after = self.game.players[acting_player_idx].chips
        reward = float(chips_after - chips_before)

        observation = self._get_observation()
        
        truncated = False 
        
        # FIX 2: Pass the new active player in the info dictionary
        info = {"current_player": self.game.activePlayer}
        return observation, reward, terminated, truncated, info

    def _get_observation(self):
        obs = np.zeros(18, dtype=np.float32)
        
        active_p = self.game.activePlayer
        player = self.game.players[active_p]
        opponent = self.game.players[1 - active_p]

        if len(player.holeCards) == 2:
            obs[0] = int(player.holeCards[0].rank)
            obs[1] = int(player.holeCards[0].suit)
            obs[2] = int(player.holeCards[1].rank)
            obs[3] = int(player.holeCards[1].suit)

        for i, card in enumerate(self.game.communityCards):
            obs[4 + (i * 2)] = int(card.rank)
            obs[5 + (i * 2)] = int(card.suit)

        obs[14] = self.game.pot
        obs[15] = self.game.currentToCall - player.currentBet
        obs[16] = player.chips
        obs[17] = opponent.chips

        return obs