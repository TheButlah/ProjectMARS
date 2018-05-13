# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import tensorflow as tf
import numpy as np
import xxhash
import project_mars as pm


# Mapping from actions to their index. Should be as many actions as `n_actions`.
action_map = {
  'place': 1,
  'nothing': 0,
}


def index_by_action_tuples(tensor, action_tuples):
  """`tensor` should have the same batch size as `action_tuples`"""
  idx = tf.range(tf.shape(action_tuples)[0])[:, tf.newaxis]

  selected = tf.gather_nd(
    tensor,
    tf.concat([idx, action_tuples], axis=1), name="Selected")

  return selected


def my_hash(state):
  return xxhash.xxh32(state).digest()


def build_numpy_state(game):
  """Constructs the numpy array for the state from the game object"""
  state = game.state
  pop = np.array(state.total_pops, dtype=np.int32)
  terrain = np.array(state.terrain, dtype=np.int32)
  plants = np.array(state.plants, dtype=np.int32)

  result = np.stack([terrain, plants, pop], axis=-1)
  result = np.expand_dims(result, axis=0)

  return result


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
  """Gets the reward signal for the current state.
  prev_serviced should be a singleton list of the previous # pops serviced."""
  total = game.get_total_serviced()
  reward = total - prev_serviced[0]
  prev_serviced[0] = total
  # print(reward)
  return reward

