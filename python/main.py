# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf
import project_mars as pm

from model import QMap
from utils import build_numpy_state, take_action, get_reward


save_path = 'saved/overnight1'

dx = 128
dy = 128
n_features = 3
n_actions = 2

box_size = 20
pop_scale = 50

# Mapping from actions to their index. Should be as many actions as `n_actions`.
action_map = {
  'place': 1,
  'nothing': 0,
}

seed = 1337

episode_length = 500


def main():
  np.random.seed(seed)

  # Initialize the model architecture
  model = QMap(
    (dx, dy, n_features),
    n_actions,
    seed=seed,
    load_model=save_path)

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

  display = pm.GameDisplay(game, box_size, pop_scale)

  # The initial game state



  total_return = 0
  prev_serviced = [0]  # List for easy mutable argument
  for step in range(episode_length):
    s = get_state(game)
    assert (s.shape[1] == dx and s.shape[2] == dy and s.shape[3] == n_features)

    display.update()

    # predicted action-value and action taken
    q, a = [np.squeeze(item) for item in model.predict_q(s)]

    take_action(game, a)  # actually take the selected action

    r = get_reward(game, prev_serviced)  # See what the reward is
    total_return += r

    print("Q-Value: %f, Action selected: %s, Reward: %d"
          % (q, str(a), r))



def get_state(game):
  return build_numpy_state(game)


if __name__ == "__main__":
  main()
