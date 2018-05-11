# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import tensorflow as tf
import numpy as np
import os

from time import strftime


class QMap:
  """Represents a "Q-Map" network.

  A Q-Map network is a convolutional neural network (CNN) that learns an
  action-value  function known as the q function. This network has been designed
  with the important assumption that there is spatial locality in *both* the
  state space and the action space. For this reason, instead of its output being
  a simple q-value as other q functions do, this model outputs a "q-map". The
  q-map represents the q-values for a set of actions that could be taken at any
  spatial position. It then indexes the q-map by the chosen action to yield
  the final q-value.

  This has the advantage of not needing to have a CNN that must output a
  very high dimensional feature space in its output. By considering the position
  of an action differently than the type of action being taken, we can create a
  CNN that produces an activation volume instead of a single prediction vector.
  This is similar to how CNNs performing semantic segmentation produce a
  prediction vector for every single spatial location rather than just a single
  final prediction vector.
  """

  def __init__(
    self, state_shape, n_actions,
    seed=None, load_model=None,
    learning_rate=0.001):
    """Initializes the architecture for the model.

    Args:

      state_shape:  A list that represents the shape of the state. For Example:
        [256, 256, 2] for a 256x256 game grid, where the first feature/channel
        represents the terrain, and the second represents the population.
        Note that it is expected that there will be a batch dimension when
        the data is actually provided to the model turing training and
        inference, but you shouldn't specify that here.

      n_actions:  An integer that is equal to the number of different actions
        that can be taken when ignoring spatial position. This will be
        equivalent to the number of feature dimensions in the resultant q-map.

      seed:  An integer used to seed the initial random state. Can be None to
        generate a new random seed.

      load_model:  If not None, then this should be a string indicating the
        checkpoint file containing the saved state of the model. It will be
        used to initialize the parameters of the model. Typically used when
        loading a pre-trained model, or resuming a previous training session.

      learning_rate:  The initial learning rate to use for gradient descent.
    """
    self._graph = tf.Graph()
    self._seed = seed
    with self._graph.as_default():
      tf.set_random_seed(seed)

      with tf.variable_scope('Inputs'):
        # Compute shape info
        state_shape = (None,) + state_shape

        # Setup placeholders
        self._states = tf.placeholder(
          tf.float32, shape=state_shape, name='States')
        self._q_targets = tf.placeholder(
          tf.float32, shape=(None,), name='Q-Targets')
        self._mu = tf.placeholder(
          tf.float32, shape=(None,), name='Mu')

        self._phase_train = tf.placeholder(
          tf.bool, shape=(), name='Phase-Train')

        self._lr = tf.placeholder(tf.float32, shape=(), name='Learning-Rate')

      with tf.variable_scope('ConvLayers'):
        # For debugging purposes, make this a simple function
        self._qmap = tf.tile(
          tf.maximum(self._states, axis=-1)[..., tf.newaxis],
          multiples=[1, 1, 1, n_actions])

        print(self._qmap.get_shape())

      with tf.variable_scope('Training'):
        '''In order to enable us to only take the gradient on the values for 
        the particular action, we need to keep the loss the same shape as the
        q-map. However we do want to average over the batch so we reduce 
        along that axis.'''
        self._loss_map = tf.reduce_mean(
          tf.nn.l2_loss(self._qmap - self._q_targets),
          axis=0)

        self._train_step = tf.train.AdamOptimizer(
          learning_rate=self._lr).minimize(self._loss_map)

      self._sess = tf.Session()
      with self._sess.as_default():
        self._saver = tf.train.Saver()

        if load_model is not None:
          print("Restoring Model...")
          load_model = os.path.abspath(load_model)
          self._saver.restore(self._sess, load_model)
          print("Model Restored!")

        else:
          print("Initializing model...")
          self._sess.run(tf.global_variables_initializer())
          print("Model Initialized!")

  def update(self, states, actions, q_targets, mu, num_epochs=1):
    """Updates the model parameters using the provided target batch.

    Args:

      states:  A numpy ndarray that represents the states of the environment.
        Should should have a shape of [batch_size, height, width, channels].

      actions: An optional numpy ndarray indicating which actions will be
        taken. If provided, should have a shape of [batch_size, 3], where the
        last dimension corresponds to a tuple of (x_pos, y_pos, action_type).
        Each element of the tuple must be within the ranges of their
        corresponding shapes defined in the initializer of the model. If
        `None`, the action selected will be the best one possible, and the
        return type of the function will be a tuple of
        `(best_qs, (best_action_tuples))`.

      q_targets:  A numpy ndarray that contains the target outputs of the q
        function. Should have a shape of [batch_size].

      mu:  A numpy ndarray that contains a weighting factor in range [0,1] for
        each state in the batch. States we care more about approximating
        accurately should be given a higher weight. For example, mu could be
        the fraction of time spent in a given state, which would mean that
        states we pass through often should be more important to approximate
        correctly.

      num_epochs:  The number of iterations over the provided batch to perform
        for this update step. Probably keep this as 1 so that the model
        doesn't become too biased due to the small size of the batch.

    Returns:
      The loss value after the update.
    """
    # TODO: Index loss and train_step by action
    with self._sess.as_default():
      # Training loop for parameter tuning
      assert(num_epochs >= 1)

      for epoch in range(num_epochs):
        _, loss_val = self._sess.run(
          [self._train_step, self._loss],
          feed_dict={
            self._states: states,
            self._q_targets: q_targets,
            self._mu: mu,
            self._phase_train: True})

      return loss_val

  def predict_q(self, states, actions=None, is_training_phase=False):
    """Uses the model to predict the q function for the given state-action pair.

    Args:

      states:  A numpy ndarray that represents the states of the environment.
        Should should have a shape of [batch_size, height, width, channels].

      actions: An optional numpy ndarray indicating which actions will be
        taken. If provided, should have a shape of [batch_size, 3], where the
        last dimension corresponds to a tuple of (x_pos, y_pos, action_type).
        Each element of the tuple must be within the ranges of their
        corresponding shapes defined in the initializer of the model. If
        `None`, the action selected will be the best one possible, and the
        return type of the function will be a tuple of
        `(best_qs, (best_action_tuples))`.

      is_training_phase:  A boolean value indicating whether we are currently
        training the network or not.

    Returns:
      A numpy ndarray of the q values, shaped [batch_size] if `actions` is not
      `None. Otherwise, provides a tuple of the q values and the actions
      selected.
    """
    # TODO: Index loss, train_step by action, figure out if action logic works
    with self._sess.as_default():

      batch_size = states.shape[0]
      assert(actions.shape[0] == batch_size if actions else True)

      qmap = self._sess.run(
        self._qmap,
        feed_dict={self._states: states, self._phase_train: is_training_phase})

      assert(qmap.shape[0] == batch_size)

      if actions is not None:
        actions = np.array(actions)
        # Numpy expects indexing format to be like [(x0, x1, x2), (y0,y1,y2)]
        # Instead of [(x0,y0), (x1,y1), (x2,y2)]
        inds_array = np.moveaxis(actions, -1, 0)
        assert(inds_array.shape == (actions.shape[1], actions.shape[0]))
        indexer = np.index_exp[:] + tuple(inds_array)
        result = qmap[indexer]
        assert(result.shape == actions.shape[0])
      else:
        # Select best action to use
        q_flat = qmap.reshape(qmap.shape[0], -1)
        idxs_flat = np.argmax(qmap, axis=-1)

        q_vals = q_flat[:, idxs_flat]

  def save_model(self, save_path=None):
    """Saves the model in the specified file.

    Args:
        save_path:  The relative path to the file. By default, it is
            saved/QMap-Year-Month-Date_Hour-Minute-Second.ckpt
    """
    with self._sess.as_default():
      print("Saving Model")
      if save_path is None:
        save_path = "saved/QMap-%s.ckpt" % strftime("%Y-%m-%d_%H-%M-%S")
      dirname = os.path.dirname(save_path)
      if dirname is not '':
        os.makedirs(dirname, exist_ok=True)
      save_path = os.path.abspath(save_path)
      path = self._saver.save(self._sess, save_path)
      print("Model successfully saved in file: %s" % path)
