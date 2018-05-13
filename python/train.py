# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf
import atexit
import project_mars as pm
import xxhash

from model import QMap
from collections import defaultdict
from utils import build_numpy_state, my_hash

save_path = "saved/overnight1"

n_episodes = 1000000
episode_length = 500

eps = 0.1  # The probability of taking a random, non-optimal action
gamma = 0.8  # The discount factor, exponential decay of future events on q val

dx = 128
dy = 128
n_features = 3
n_actions = 2

# Mapping from actions to their index. Should be as many actions as `n_actions`.
action_map = {
  'place': 1,
  'nothing': 0,
}

seed = 1337


def exit_fn(model, file):
  model.save_model(save_path)
  file.close()



def main():
  np.random.seed(seed)

  # Initialize the model architecture
  model = QMap(
    (dx, dy, n_features),
    n_actions,
    seed=seed)

  f = open(save_path+'.log', 'w')

  # Ensure we have our progress saved
  atexit.register(lambda: exit_fn(model, f))


  # Learn from simulated experience
  for episode in range(n_episodes):
    game = pm.Game(
      dx=dx,
      dy=dy,
      number_of_turns=episode_length,
      default_capacity=25,
      servable_distance=3.0,
      initial_cost=50.0,
      operating_cost=25.0,
      profit_margin=5.0,
      unserviced_penalty=1.0)

    # Keeps track of the number of times each state is visited
    state_counts = defaultdict(lambda: 0)

    # The initial game state
    s = get_state(game, state_counts)
    assert(s.shape[1] == dx and s.shape[2] == dy and s.shape[3] == n_features)

    loss_avg = 0
    total_return = 0
    prev_serviced = [0]  # List for easy mutable argument
    for step in range(episode_length):

      q, a = compute_action_value(model, s)  # action-value and action taken

      take_action(game, a)  # actually take the selected action
      s_prime = get_state(game, state_counts)  # See what the new state is
      r = get_reward(game, prev_serviced)  # See what the reward is
      total_return += r

      # Retroactively predict the value of the previous state using the reward
      # The terminal state should have a value of 0, so we add a check for that
      if step < episode_length - 1:
        q_prime, _ = model.predict_q(s_prime)
        q_prime *= gamma
        q_prime += r
      else:
        q_prime = np.array(r, dtype=np.float32, ndmin=1)

      mu = get_mu(state_counts, s)  # Weight for importance of `s`

      # Gradient update for model towards q_prime
      loss = model.update(s, a, q_prime, mu)
      loss_avg += loss/episode_length

      #print('q_prime, q, reward', q_prime, q, r)

      s = s_prime  # prep for the next step

    print("Completed episode %d. Avg. loss: %f, Total Return: %f "
          % (episode, loss_avg, total_return))
    f.write("%d,%f,%d\n" % (episode, loss_avg, total_return))
    f.flush()



def compute_action_value(model, state):
  """Computes the q value and also action taken following an epsilon-greedy
  policy. Epsilon greedy policies take a non-optimal action with probability
  equal to Epsilon. This ensures theoretical convergence and exploration."""
  # Evaluate eps-greedy policy
  if np.random.rand() < eps:
    a = np.random.uniform(high=[dx, dy, n_actions], size=(3,))[np.newaxis, :]
    a = a.astype(np.int32, copy=False)
    q = model.predict_q(state, a)
  else:
    q, a = model.predict_q(state)
  return q, a


def get_mu(state_counts, state):
  """Computes the weight for the particular state. States explored more
  frequently will have a higher weight, which encourages the model to place
  more importance on learning those states accurately. Should sum to 1."""
  state_hashes = [my_hash(s) for s in state]
  return np.array([state_counts[h]/state_counts['all'] for h in state_hashes])


def take_action(game, action):
  """Interprets the provided action tuple and steps the game state."""
  action = np.squeeze(action)
  action_type = action[-1]
  if action_type == action_map['nothing']:
    game.step(False, pm.Coord(0, 0))
  else:
    assert(action_type == action_map['place'])
    game.step(True, pm.Coord(action[0], action[1]))


def get_reward(game, prev_serviced):
  """Gets the reward signal for the current state"""
  total = game.get_total_serviced()
  reward = total - prev_serviced[0]
  prev_serviced[0] = total
  # print(reward)
  return reward


def get_state(game, state_counts):
  """Returns a numpy array representing the game state, and updates
  associated info like the number of times we have seen that state this
  episode. Note that the state *will* have a batch dimension, even if its
  just a singleton dimension. """

  state = build_numpy_state(game)

  # simple hashing of state
  h = my_hash(state)

  state_counts[h] += 1
  state_counts['all'] += 1
  return state


if __name__ == "__main__":
  main()
