# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf


def main():
  """kill me now"""
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
