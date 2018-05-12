# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf
from model import QMap


def main():
  state_shape = (3, 3, 10)
  model = QMap(state_shape, 2)
  initial_state = np.zeros((1,)+state_shape)
  q = model.predict_q(initial_state, np.zeros((1, 3)))
  print('Q:', q)
  q, selected = model.predict_q(initial_state)
  print('Q:', q, 'Selected:', selected)


def main2():
  """Figuring out tf.unravel_index"""
  x = tf.constant(np.array([
    [[0,1],
    [0,0]],

    [[0,0],
    [0,1]],

    [[0,0],
     [1,0]]
  ]))
  print(x)
  flattened = tf.reshape(x, (-1, 4))
  arg_max = tf.argmax(flattened, axis=-1, output_type=tf.int32)
  max_idx = tf.unravel_index(arg_max, (2,2))

  sess = tf.InteractiveSession()
  print('arg_max.shape', arg_max.shape)
  print('max_idx.shape', max_idx.shape)
  print('arg_max.eval()', arg_max.eval())
  print('max_idx.eval()', max_idx.eval())


def main1():
  """kill me now as I try to figure out tf.gather_nd"""
  x = tf.constant(np.arange(24).reshape(4, 3, 2))
  idx = tf.constant(np.array([
    [0,0],
    [2,0],
    [0,1],
    [2,1],

  ]))

  idx_cat = tf.concat([tf.range(4, dtype=tf.int64)[:, np.newaxis], idx], axis=1)

  result = tf.gather_nd(x, idx_cat)

  sess = tf.InteractiveSession()

  print(x.eval(), '\n\n')
  print(idx_cat.eval(), '\n\n')
  print(result.eval())


if __name__ == "__main__":
  main()
