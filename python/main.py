# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf
import atexit
import project_mars as pm

from model import QMap
from collections import defaultdict

n_episodes = 1000000
episode_length = 500

eps = 0.1  # The probability of taking a random, non-optimal action

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


def main():
  np.random.seed(seed)

  # Initialize the model architecture
  model = QMap(
    (dx, dy, n_features),
    n_actions,
    seed=seed)

  atexit.register(model.save_model)  # Ensure we have our progress saved


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

    for step in range(episode_length):

      q, a = compute_action_value(model, s)  # action-value and action taken

      take_action(game, a)  # actually take the selected action
      s_prime = get_state(game, state_counts)  # See what the new state is
      r = get_reward(game)  # See what the reward is

      # Retroactively predict the value of the previous state using the reward
      # The terminal state should have a value of 0, so we add a check for that
      if step < episode_length - 1:
        q_prime, _ = model.predict_q(s_prime)
        q_prime += r
      else:
        q_prime = np.array(r, dtype=np.float32, ndmin=1)

      mu = get_mu(state_counts, s)  # Weight for importance of `s`
      model.update(s, a, q_prime, mu)  # Gradient update for model towards `q'`

      s = s_prime  # prep for the next step

    print("Completed episode %d" % episode)


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
  more importance on learning those states accurately."""
  state_hashes = [hash(str(s)) for s in state]
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


def get_reward(game):
  """Gets the reward signal for the current state"""
  return game.get_reward()


def get_state(game, state_counts):
  """Returns a numpy array representing the game state, and updates
  associated info like the number of times we have seen that state this
  episode. Note that the state *will* have a batch dimension, even if its
  just a singleton dimension. """

  # TODO: Get ndarray from C++ instead of just an array of zeros.
  # state = game.get_state()
  state = np.zeros((1, dx, dy, n_features), dtype=np.float32)

  # simple hashing of state
  h = hash(str(state))

  state_counts[h] += 1
  state_counts['all'] += 1
  return state


if __name__ == "__main__":
  main()
