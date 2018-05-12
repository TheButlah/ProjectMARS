# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import tensorflow as tf
import numpy as np


def action_tuples_to_mask(action_tuples, action_shape):
  """`action_tuples` should have shape [None, 3]"""

  sparse = tf.SparseTensor(
    indices=action_tuples,
    values=True,
    dense_shape=action_tuples[1:])

  print('sparse:', sparse)
  return sparse
