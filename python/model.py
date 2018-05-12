# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import tensorflow as tf
import numpy as np
import os

from time import strftime
from utils import *
from layers import *


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
    self, state_shape, n_actions, seed=None, load_model=None):
    """Initializes the architecture for the model.

    Args:

      state_shape:  A list that represents the shape of the state. For Example:
        [256, 256, 2] for a 256x256 game grid, where the first feature/channel
        represents the terrain, and the second represents the population.
        Note that it is expected that there will be a batch dimension when
        the data is actually provided to the model turing training and
        inference, but you shouldn't specify that here.

      n_actions:  An integer that is equal to the number of different
        types of actions that can be taken when ignoring spatial position.
        This will be equivalent to the number of feature dimensions in the
        resultant q-map.

      seed:  An integer used to seed the initial random state. Can be None to
        generate a new random seed.

      load_model:  If not None, then this should be a string indicating the
        checkpoint file containing the saved state of the model. It will be
        used to initialize the parameters of the model. Typically used when
        loading a pre-trained model, or resuming a previous training session.
    """
    self._graph = tf.Graph()
    self._seed = seed
    with self._graph.as_default():
      tf.set_random_seed(seed)


      with tf.variable_scope('Inputs'):
        # Compute shape info
        state_shape = (None,) + tuple(state_shape)
        action_shape = state_shape[:-1] + (n_actions,)

        #####################
        # Data placeholders #
        #####################

        # Input placeholder for batch of states to evaluate the q function for
        self._states = tf.placeholder(
          tf.float32, shape=state_shape, name='States')

        # Targets for the q function. This is just a list, not a map
        self._q_targets = tf.placeholder(
          tf.float32, shape=(None,), name='Q-Targets')

        # List of tuples describing the actions to evaluate the q function for.
        # This may be not provided if we are instead computing the best action.
        self._action_tuples = tf.placeholder(
          tf.int32, shape=(None, 3), name='Action-Tuples')

        # The weighting distribution over the samples. States we care more about
        # approximating accurately should be given a higher weight.
        self._mu = tf.placeholder(
          tf.float32, shape=(None,), name='Mu')

        ####################
        # Hyper-Parameters #
        ####################

        self._lr = tf.placeholder(tf.float32, shape=(), name='Learning-Rate')

        #################
        # Training Info #
        #################

        # Whether we are currently trying to train the model. Will affect
        # dropout regularization.
        self._phase_train = tf.placeholder(
          tf.bool, shape=(), name='Phase-Train')

        # Whether we want to pick the best action automatically, or use the
        # provided list of actions. By default, we use the provided actions.
        self._choose_actions = tf.placeholder_with_default(
          tf.constant(False), shape=(), name='Choose-Actions')


      with tf.variable_scope('Preprocessing'):
        '''# Expand action_tuples into full binary masks
        self._action_mask = action_tuples_to_mask(
          self._action_tuples, action_shape)'''
        pass


      with tf.variable_scope('Layers'):
        pass


      with tf.variable_scope('Output'):
        # For debugging purposes, make this a simple function
        self._qmap = tf.tile(
          tf.reduce_max(self._states, axis=-1)[..., tf.newaxis],
          multiples=[1, 1, 1, n_actions])

        print('qmap shape:', self._qmap.get_shape())

        # TODO: use tf.cond to decide whether to choose optimal action
        # TODO: Confirm this broadcasts properly
        # Should end up with shape [batch_size]
        self._q = index_by_action_tuples(self._qmap, self._action_tuples)

        print('q:', self._q)


      with tf.variable_scope('Training'):
        self._loss = tf.reduce_mean(
          tf.nn.l2_loss(self._q - self._q_targets))

        self._train_step = tf.train.AdamOptimizer(
          learning_rate=self._lr).minimize(self._loss)


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


  def update(self, states, actions, q_targets, mu, num_epochs=1, lr=0.001):
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
        `(loss_val, (best_action_tuples))`.

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

      lr:  The base learning rate to use for gradient descent.

    Returns:
      The loss value after the update, or if `actions` was `None`, a tuple of
      the loss and the selected actions.
    """
    with self._sess.as_default():
      assert(num_epochs >= 1)

      batch_size = states.shape[0]
      assert(actions.shape[0] == batch_size if actions else True)

      feed_dict = {
        self._states: states,
        self._q_targets: q_targets,
        self._mu: mu,
        self._phase_train: True,
        self._lr: lr}

      # Choose whether to compute the best action or use the provided ones
      if actions is None:
        feed_dict[self._choose_actions] = True
      else:
        feed_dict[self._action_tuples] = actions

      # Training step(s)
      for epoch in range(num_epochs):
        _, loss_val = self._sess.run(
          [self._train_step, self._loss],
          feed_dict=feed_dict)

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
    with self._sess.as_default():

      batch_size = states.shape[0]
      assert(actions.shape[0] == batch_size if actions else True)

      feed_dict = {
        self._states: states,
        self._phase_train: is_training_phase}

      # Choose whether to compute the best action or use the provided ones
      if actions is None:
        feed_dict[self._choose_actions] = True
      else:
        feed_dict[self._action_tuples] = actions

      # TODO: if actions is None, also return selected action

      q_pred = self._sess.run(self._q, feed_dict=feed_dict)

      return q_pred


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
