import tensorflow as tf
import numpy as np
import os

from time import strftime


class DQN:

  def __init__(self):
    pass

    def update(self, state, target_q, mu, num_epochs=1):
      """Updates the model parameters using the provided target batch.

      Args:
          state:  A numpy ndarray that represents the state of the environment(s). Should should have a shape of
            [batch_size, height, width, channels].

          target_q:  A numpy ndarray that contains the target output of the q function. Should have a shape of
            [batch_size, n_actions]. Note that `n_actions` does not take into consideration position as part of the
            action space. Instead it should be a probability vector over a number of actions that could be taken at any
            given position.

          mu:  A numpy ndarray that contains a weighting factor for each sample. For example, mu could be the fraction
            of time spent in the given state.

          num_epochs:  The number of iterations over the provided batch to perform for this update step. Probably keep
            this as 1 so that the model doesn't become too biased due to the small size of the batch.

      Returns:
          The loss value after training
      """
      with self._sess.as_default():
        # Training loop for parameter tuning
        assert(num_epochs >= 1)

        for epoch in range(num_epochs):
          _, loss_val = self._sess.run(
            [self._train_step, self._loss],
            feed_dict={self._state: state, self._target_q: target_q, self._mu: mu, self._phase_train: True})

        return loss_val

  def predict_q(self, state):
    """Uses the model to predict the q values for state.

    Args:
      state:  A numpy ndarray that represents the state of the environment(s). Should should have a shape of
        [batch_size, height, width, channels].

    Returns:
      A numpy ndarray of the q values, shaped [batch_size]
    """
    with self._sess.as_default():
      return self._sess.run(self._q, feed_dict={self._state: state, self._phase_train: False})

  def save_model(self, save_path=None):
    """Saves the model in the specified file.

    Args:
        save_path:  The relative path to the file. By default, it is
            saved/DQN-Year-Month-Date_Hour-Minute-Second.ckpt
    """
    with self._sess.as_default():
      print("Saving Model")
      if save_path is None:
        save_path = "saved/DQN-%s.ckpt" % strftime("%Y-%m-%d_%H-%M-%S")
      dirname = os.path.dirname(save_path)
      if dirname is not '':
        os.makedirs(dirname, exist_ok=True)
      save_path = os.path.abspath(save_path)
      path = self._saver.save(self._sess, save_path)
      print("Model successfully saved in file: %s" % path)