# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import tensorflow as tf
import numpy as np
import xxhash


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
  state = game.state
  pop = np.array(state.total_pops, dtype=np.int32)
  terrain = np.array(state.terrain, dtype=np.int32)
  plants = np.array(state.plants, dtype=np.int32)

  result = np.stack([terrain, plants, pop], axis=-1)
  result = np.expand_dims(result, axis=0)

  return result

